/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "textconverter.h"
#include "emoticons/emojimanager.h"
#include "messagecache.h"
#include "messages/message.h"
#include "ruqola_debug.h"
#include "utils.h"

#include "ktexttohtmlfork/ruqolaktexttohtml.h"
#include "syntaxhighlightingmanager.h"
#include "texthighlighter.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

#include <KColorScheme>

namespace
{
/// check if the @p str contains an uneven number of backslashes before @p pos
bool isEscaped(const QString &str, int pos)
{
    int backslashes = 0;
    while (pos > 0 && str[pos - 1] == QLatin1Char('\\')) {
        ++backslashes;
        --pos;
    }
    // even number of escapes means the
    return backslashes % 2 == 1;
}

int findNonEscaped(const QString &str, const QString &regionMarker, int startFrom)
{
    while (true) {
        const int index = str.indexOf(regionMarker, startFrom);
        if (index == -1) {
            return -1;
        } else if (isEscaped(str, index)) {
            startFrom = index + regionMarker.size();
            continue;
        }
        return index;
    }
    Q_UNREACHABLE();
}

template<typename InRegionCallback, typename OutsideRegionCallback>
void iterateOverRegions(const QString &str, const QString &regionMarker, InRegionCallback &&inRegion, OutsideRegionCallback &&outsideRegion)
{
    int startFrom = 0;
    const auto markerSize = regionMarker.size();
    while (true) {
        const int startIndex = findNonEscaped(str, regionMarker, startFrom);
        if (startIndex == -1) {
            break;
        }

        const int endIndex = findNonEscaped(str, regionMarker, startIndex + markerSize);
        if (endIndex == -1) {
            break;
        }

        const auto codeBlock = str.mid(startIndex + markerSize, endIndex - startIndex - markerSize).trimmed();

        outsideRegion(str.mid(startFrom, startIndex - startFrom));
        startFrom = endIndex + markerSize;

        inRegion(codeBlock);
    }
    outsideRegion(str.mid(startFrom));
}

QString markdownToRichText(const QString &markDown)
{
    if (markDown.isEmpty()) {
        return QString();
    }

    // qCDebug(RUQOLA_LOG) << "BEFORE markdownToRichText "<<markDown;
    QString str = markDown;

    const RuqolaKTextToHTML::Options convertFlags = RuqolaKTextToHTML::HighlightText | RuqolaKTextToHTML::ConvertPhoneNumbers;
    str = RuqolaKTextToHTML::convertToHtml(str, convertFlags);

    // substitute "[example.com](<a href="...">...</a>)" style urls
    str = Utils::convertTextWithUrl(str);

    return str;
}

QString generateRichText(const QString &str, const QString &username, const QStringList &highlightWords)
{
    QString newStr = markdownToRichText(str);
    KColorScheme colorScheme;
    const auto userHighlightForegroundColor = colorScheme.foreground(KColorScheme::PositiveText).color().name();
    const auto userHighlightBackgroundColor = colorScheme.background(KColorScheme::PositiveBackground).color().name();
    if (!highlightWords.isEmpty()) {
        struct HrefPos {
            int start = 0;
            int end = 0;
        };
        QList<HrefPos> lstPos;
        static const QRegularExpression regularExpressionAHref(QStringLiteral("(<a href=\".*\">)"));
        QRegularExpressionMatchIterator userIteratorHref = regularExpressionAHref.globalMatch(newStr);
        while (userIteratorHref.hasNext()) {
            const QRegularExpressionMatch match = userIteratorHref.next();
            HrefPos pos;
            pos.start = match.capturedStart(1);
            pos.end = match.capturedEnd(1);
            lstPos.append(std::move(pos));
        }

        for (const QString &word : highlightWords) {
            const QRegularExpression exp(QStringLiteral("(\\b%1\\b)").arg(word), QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatchIterator userIterator = exp.globalMatch(newStr);
            int offset = 0;
            while (userIterator.hasNext()) {
                const QRegularExpressionMatch match = userIterator.next();
                const QString word = match.captured(1);
                bool inAnUrl = false;
                const int matchCapturedStart = match.capturedStart(1);
                for (const HrefPos &hrefPos : lstPos) {
                    if ((matchCapturedStart > hrefPos.start) && (matchCapturedStart < hrefPos.end)) {
                        inAnUrl = true;
                        break;
                    }
                }
                if (inAnUrl) {
                    continue;
                }
                const QString replaceStr =
                    QStringLiteral("<a style=\"color:%2;background-color:%3;\">%1</a>").arg(word, userHighlightForegroundColor, userHighlightBackgroundColor);
                newStr.replace(matchCapturedStart + offset, word.length(), replaceStr);
                // We added a new string => increase offset
                offset += replaceStr.length() - word.length();
            }
        }
    }
    static const QRegularExpression regularExpressionUser(QStringLiteral("(^|\\s+)@([\\w._-]+)"));
    QRegularExpressionMatchIterator userIterator = regularExpressionUser.globalMatch(newStr);

    const auto userMentionForegroundColor = colorScheme.foreground(KColorScheme::ActiveText).color().name();
    const auto userMentionBackgroundColor = colorScheme.background(KColorScheme::ActiveBackground).color().name();
    while (userIterator.hasNext()) {
        const QRegularExpressionMatch match = userIterator.next();
        const QStringRef word = match.capturedRef(2);
        // Highlight only if it's yours
        if (word == username) {
            newStr.replace(QLatin1Char('@') + word,
                           QStringLiteral("<a href=\'ruqola:/user/%1\' style=\"color:%2;background-color:%3;\">@%1</a>")
                               .arg(word.toString(), userMentionForegroundColor, userMentionBackgroundColor));
        } else {
            newStr.replace(QLatin1Char('@') + word, QStringLiteral("<a href=\'ruqola:/user/%1\'>@%1</a>").arg(word));
        }
    }
    static const QRegularExpression regularExpressionRoom(QStringLiteral("(^|\\s+)#([\\w._-]+)"));
    QRegularExpressionMatchIterator roomIterator = regularExpressionRoom.globalMatch(newStr);
    while (roomIterator.hasNext()) {
        const QRegularExpressionMatch match = roomIterator.next();
        const QStringRef word = match.capturedRef(2);
        newStr.replace(QLatin1Char('#') + word, QStringLiteral("<a href=\'ruqola:/room/%1\'>#%1</a>").arg(word));
    }

    return newStr;
}
}

QString TextConverter::convertMessageText(const QString &_str,
                                          const QString &userName,
                                          const QVector<Message> &allMessages,
                                          const QStringList &highlightWords,
                                          EmojiManager *emojiManager,
                                          MessageCache *messageCache,
                                          QString &needUpdateMessageId)
{
    if (!emojiManager) {
        qCWarning(RUQOLA_LOG) << "Emojimanager is null";
    }

    QString quotedMessage;

    QString str = _str;
    // TODO we need to look at room name too as we can have it when we use "direct reply"
    if (str.contains(QLatin1String("[ ](http"))) { // ## is there a better way?
        const int startPos = str.indexOf(QLatin1Char('('));
        const int endPos = str.indexOf(QLatin1Char(')'));
        const QString url = str.mid(startPos + 1, endPos - startPos - 1);
        // URL example https://HOSTNAME/channel/all?msg=3BR34NSG5x7ZfBa22
        const QString messageId = url.mid(url.indexOf(QLatin1String("msg=")) + 4);
        // qCDebug(RUQOLA_LOG) << "Extracted messageId" << messageId;
        auto it = std::find_if(allMessages.cbegin(), allMessages.cend(), [messageId](const Message &msg) {
            return msg.messageId() == messageId;
        });
        if (it != allMessages.cend()) {
            const QString text = convertMessageText((*it).text(), userName, allMessages, highlightWords, emojiManager, messageCache, needUpdateMessageId);
            quotedMessage = Utils::formatQuotedRichText(text);
            str = str.left(startPos - 3) + str.mid(endPos + 1);
        } else {
            if (messageCache) {
                // TODO allow to reload index when we loaded message
                Message *msg = messageCache->messageForId(messageId);
                if (msg) {
                    const QString text =
                        convertMessageText(msg->text(), userName, allMessages, highlightWords, emojiManager, messageCache, needUpdateMessageId);
                    quotedMessage = Utils::formatQuotedRichText(text);
                    str = str.left(startPos - 3) + str.mid(endPos + 1);
                } else {
                    qCDebug(RUQOLA_LOG) << "Quoted message" << messageId << "not found"; // could be a very old one
                    needUpdateMessageId = messageId;
                }
            }
        }
    }

    QString richText;
    QTextStream richTextStream(&richText);
    KColorScheme scheme;
    const auto codeBackgroundColor = scheme.background(KColorScheme::AlternateBackground).color();
    const auto codeBorderColor = scheme.foreground(KColorScheme::InactiveText).color().name();

    QString highlighted;
    QTextStream stream(&highlighted);
    TextHighlighter highlighter(&stream);
    const auto useHighlighter = SyntaxHighlightingManager::self()->syntaxHighlightingInitialized();

    if (useHighlighter) {
        auto &repo = SyntaxHighlightingManager::self()->repo();
        highlighter.setTheme(codeBackgroundColor.lightness() < 128 ? repo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                   : repo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    }
    auto highlight = [&](const QString &codeBlock) {
        if (!useHighlighter) {
            return codeBlock;
        }
        stream.reset();
        stream.seek(0);
        highlighted.clear();
        highlighter.highlight(codeBlock);
        return highlighted;
    };

    auto addCodeChunk = [&](QString chunk) {
        const auto language = [&]() {
            const auto newline = chunk.indexOf(QLatin1Char('\n'));
            if (newline == -1) {
                return QString();
            }
            return chunk.left(newline);
        }();

        auto definition = SyntaxHighlightingManager::self()->def(language);
        if (definition.isValid()) {
            chunk.remove(0, language.size() + 1);
        } else {
            definition = SyntaxHighlightingManager::self()->defaultDef();
        }

        highlighter.setDefinition(std::move(definition));
        // Qt's support for borders is limited to tables, so we have to jump through some hoops...
        richTextStream << QLatin1String("<table><tr><td style='background-color:") << codeBackgroundColor.name()
                       << QLatin1String("; padding: 5px; border: 1px solid ") << codeBorderColor << QLatin1String("'>") << highlight(chunk)
                       << QLatin1String("</td></tr></table>");
    };

    auto addInlineCodeChunk = [&](const QString &chunk) {
        richTextStream << QLatin1String("<code style='background-color:") << codeBackgroundColor.name() << QLatin1String("'>") << chunk.toHtmlEscaped()
                       << QLatin1String("</code>");
    };

    auto addTextChunk = [&](const QString &chunk) {
        auto htmlChunk = generateRichText(chunk, userName, highlightWords);
        if (emojiManager) {
            emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << htmlChunk;
    };

    auto addNonCodeChunk = [&](QString chunk) {
        chunk = chunk.trimmed();
        if (chunk.isEmpty()) {
            return;
        }

        richTextStream << QLatin1String("<div>");
        iterateOverRegions(chunk, QStringLiteral("`"), addInlineCodeChunk, addTextChunk);
        richTextStream << QLatin1String("</div>");
    };

    iterateOverRegions(str, QStringLiteral("```"), addCodeChunk, addNonCodeChunk);

    return QLatin1String("<qt>") + quotedMessage + richText + QLatin1String("</qt>");
}
