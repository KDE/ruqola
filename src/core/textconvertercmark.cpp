/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textconvertercmark.h"
#include "colorsandmessageviewstyle.h"
#include "emoticons/emojimanager.h"
#include "messagecache.h"
#include "messages/message.h"
#include "ruqola_texttohtml_debug.h"
#include "utils.h"

#include "ktexttohtmlfork/ruqolaktexttohtml.h"
#include "syntaxhighlightingmanager.h"
#include "texthighlighter.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

#include <KColorScheme>
using namespace Qt::Literals::StringLiterals;
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
int findNewLineOrEndLine(const QString &str, const QString &regionMarker, int startFrom)
{
    const int index = str.indexOf(regionMarker, startFrom);
    if (index == -1) {
        return str.length() - 1;
    } else {
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

template<typename InRegionCallback, typename OutsideRegionCallback, typename NewLineCallBack>
void iterateOverEndLineRegions(const QString &str,
                               const QString &regionMarker,
                               InRegionCallback &&inRegion,
                               OutsideRegionCallback &&outsideRegion,
                               NewLineCallBack &&newLine)
{
    // We have quote text if text start with > or we have "\n>"
    if (str.startsWith(regionMarker) || str.contains(QStringLiteral("\n") + regionMarker)) {
        int startFrom = 0;
        const auto markerSize = regionMarker.size();
        bool hasCode = false;
        while (true) {
            const int startIndex = findNonEscaped(str, regionMarker, startFrom);
            if (startIndex == -1) {
                break;
            }

            const int endIndex = findNewLineOrEndLine(str, QStringLiteral("\n"), startIndex + markerSize);
            if (endIndex == -1) {
                break;
            }
            QStringView codeBlock = QStringView(str).mid(startIndex + markerSize, endIndex - startIndex).trimmed();
            if (codeBlock.endsWith(regionMarker)) {
                codeBlock.chop(regionMarker.size());
            }
            if (hasCode) {
                newLine();
            }
            const QStringView midCode = QStringView(str).mid(startFrom, startIndex - startFrom);
            outsideRegion(midCode.toString());
            startFrom = endIndex + markerSize;

            inRegion(codeBlock.toString());
            if (!codeBlock.isEmpty()) {
                hasCode = true;
            }
        }
        const QString afterstr = str.mid(startFrom);
        outsideRegion(afterstr);
    } else {
        outsideRegion(str);
    }
}

QString markdownToRichText(const QString &markDown)
{
    if (markDown.isEmpty()) {
        return {};
    }

    qCDebug(RUQOLA_TEXTTOHTML_LOG) << "BEFORE markdownToRichText " << markDown;
    QString str = markDown;

    const RuqolaKTextToHTML::Options convertFlags = RuqolaKTextToHTML::HighlightText | RuqolaKTextToHTML::ConvertPhoneNumbers;
    str = RuqolaKTextToHTML::convertToHtml(str, convertFlags);
    qCDebug(RUQOLA_TEXTTOHTML_LOG) << " AFTER convertToHtml " << str;
    // substitute "[example.com](<a href="...">...</a>)" style urls
    str = Utils::convertTextWithUrl(str);
    // Substiture "- [ ] foo" and "- [x] foo" to checkmark
    str = Utils::convertTextWithCheckMark(str);
    // Substiture # header
    str = Utils::convertTextHeaders(str);
    // Substiture unsorted list
    str = Utils::convertTextUnsortedList(str);
    qCDebug(RUQOLA_TEXTTOHTML_LOG) << " AFTER convertTextWithUrl " << str;

    return str;
}

QString generateRichText(const QString &str,
                         const QString &username,
                         const QStringList &highlightWords,
                         const QMap<QString, QByteArray> &mentions,
                         const QMap<QString, QByteArray> &channels,
                         const QString &searchedText)
{
    QString newStr = markdownToRichText(str);
    static const QRegularExpression regularExpressionAHref(QStringLiteral("(<a href=\'.*\'>|<a href=\".*\">)"));
    struct HrefPos {
        int start = 0;
        int end = 0;
    };
    QList<HrefPos> lstPos;
    {
        QRegularExpressionMatchIterator userIteratorHref = regularExpressionAHref.globalMatch(newStr);
        while (userIteratorHref.hasNext()) {
            const QRegularExpressionMatch match = userIteratorHref.next();
            HrefPos pos;
            pos.start = match.capturedStart(1);
            pos.end = match.capturedEnd(1);
            lstPos.append(std::move(pos));
        }

        static const QRegularExpression regularExpressionRoom(QStringLiteral("(^|\\s+)#([\\w._-]+)"), QRegularExpression::UseUnicodePropertiesOption);
        QRegularExpressionMatchIterator roomIterator = regularExpressionRoom.globalMatch(newStr);
        while (roomIterator.hasNext()) {
            const QRegularExpressionMatch match = roomIterator.next();
            const QStringView word = match.capturedView(2);
            bool inAnUrl = false;
            const int matchCapturedStart = match.capturedStart(2);
            for (const HrefPos &hrefPos : lstPos) {
                if ((matchCapturedStart > hrefPos.start) && (matchCapturedStart < hrefPos.end)) {
                    inAnUrl = true;
                    break;
                }
            }
            if (inAnUrl) {
                continue;
            }
            QByteArray roomIdentifier = channels.value(word.toString());
            if (roomIdentifier.isEmpty()) {
                roomIdentifier = word.toString().toLatin1();
            }
            newStr.replace(QLatin1Char('#') + word.toString(),
                           QStringLiteral("<a href=\'ruqola:/room/%2\'>#%1</a>").arg(word, QString::fromLatin1(roomIdentifier)));
        }
    }

    if (!highlightWords.isEmpty()) {
        const auto userHighlightForegroundColor = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::PositiveText).color().name();
        const auto userHighlightBackgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::PositiveBackground).color().name();
        lstPos.clear();
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

    if (!searchedText.isEmpty()) {
        const auto userHighlightForegroundColor = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NeutralText).color().name();
        const auto userHighlightBackgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::NeutralBackground).color().name();
        lstPos.clear();
        QRegularExpressionMatchIterator userIteratorHref = regularExpressionAHref.globalMatch(newStr);
        while (userIteratorHref.hasNext()) {
            const QRegularExpressionMatch match = userIteratorHref.next();
            HrefPos pos;
            pos.start = match.capturedStart(1);
            pos.end = match.capturedEnd(1);
            lstPos.append(std::move(pos));
        }

        const QRegularExpression exp(QStringLiteral("(%1)").arg(searchedText), QRegularExpression::CaseInsensitiveOption);
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
    static const QRegularExpression regularExpressionUser(QStringLiteral("(^|\\s+)@([\\w._-]+)"), QRegularExpression::UseUnicodePropertiesOption);
    QRegularExpressionMatchIterator userIterator = regularExpressionUser.globalMatch(newStr);

    const auto userMentionForegroundColor = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NegativeText).color().name();
    const auto userMentionBackgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::NegativeBackground).color().name();
    const auto hereAllMentionBackgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::NeutralBackground).color().name();
    const auto hereAllMentionForegroundColor = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NeutralText).color().name();
    while (userIterator.hasNext()) {
        const QRegularExpressionMatch match = userIterator.next();
        const QStringView word = match.capturedView(2);
        // Highlight only if it's yours

        const QByteArray userIdentifier = mentions.value(word.toString());
        QString wordFromUserIdentifier = QString::fromLatin1(userIdentifier);
        if (userIdentifier.isEmpty()) {
            wordFromUserIdentifier = word.toString();
        }
        if (word == username) {
            newStr.replace(QLatin1Char('@') + word.toString(),
                           QStringLiteral("<a href=\'ruqola:/user/%4\' style=\"color:%2;background-color:%3;font-weight:bold\">@%1</a>")
                               .arg(word.toString(), userMentionForegroundColor, userMentionBackgroundColor, wordFromUserIdentifier));

        } else {
            if (!Utils::validUser(wordFromUserIdentifier)) { // here ? all ?
                newStr.replace(QLatin1Char('@') + word.toString(),
                               QStringLiteral("<a style=\"color:%2;background-color:%3;font-weight:bold\">%1</a>")
                                   .arg(word.toString(), hereAllMentionForegroundColor, hereAllMentionBackgroundColor));
            } else {
                newStr.replace(QLatin1Char('@') + word.toString(), QStringLiteral("<a href=\'ruqola:/user/%2\'>@%1</a>").arg(word, wordFromUserIdentifier));
            }
        }
    }

    return newStr;
}
}

QString TextConverterCMark::convertMessageText(const TextConverter::ConvertMessageTextSettings &settings, QByteArray &needUpdateMessageId, int &recusiveIndex)
{
    if (!settings.emojiManager) {
        qCWarning(RUQOLA_TEXTTOHTML_LOG) << "Emojimanager is null";
    }

    QString quotedMessage;

    QString str = settings.str;
    // TODO we need to look at room name too as we can have it when we use "direct reply"
    if (str.contains("[ ](http"_L1)
        && (settings.maximumRecursiveQuotedText == -1 || (settings.maximumRecursiveQuotedText > recusiveIndex))) { // ## is there a better way?
        const int startPos = str.indexOf(QLatin1Char('('));
        const int endPos = str.indexOf(QLatin1Char(')'));
        const QString url = str.mid(startPos + 1, endPos - startPos - 1);
        // URL example https://HOSTNAME/channel/all?msg=3BR34NSG5x7ZfBa22
        const QByteArray messageId = url.mid(url.indexOf("msg="_L1) + 4).toLatin1();
        // qCDebug(RUQOLA_TEXTTOHTML_LOG) << "Extracted messageId" << messageId;
        auto it = std::find_if(settings.allMessages.cbegin(), settings.allMessages.cend(), [messageId](const Message &msg) {
            return msg.messageId() == messageId;
        });
        if (it != settings.allMessages.cend()) {
            const TextConverter::ConvertMessageTextSettings newSetting(QLatin1Char('@') + (*it).username() + QStringLiteral(": ") + (*it).text(),
                                                                       settings.userName,
                                                                       settings.allMessages,
                                                                       settings.highlightWords,
                                                                       settings.emojiManager,
                                                                       settings.messageCache,
                                                                       (*it).mentions(),
                                                                       (*it).channels(),
                                                                       settings.searchedText,
                                                                       settings.maximumRecursiveQuotedText);
            recusiveIndex++;
            const QString text = TextConverterCMark::convertMessageText(newSetting, needUpdateMessageId, recusiveIndex);
            Utils::QuotedRichTextInfo info;
            info.url = url;
            info.richText = text;
            info.displayTime = (*it).dateTime();
            quotedMessage = Utils::formatQuotedRichText(std::move(info));
            str = str.left(startPos - 3) + str.mid(endPos + 1);
        } else {
            if (settings.messageCache) {
                // TODO allow to reload index when we loaded message
                Message *msg = settings.messageCache->messageForId(messageId);
                if (msg) {
                    const TextConverter::ConvertMessageTextSettings newSetting(msg->text(),
                                                                               settings.userName,
                                                                               settings.allMessages,
                                                                               settings.highlightWords,
                                                                               settings.emojiManager,
                                                                               settings.messageCache,
                                                                               msg->mentions(),
                                                                               msg->channels(),
                                                                               settings.searchedText,
                                                                               settings.maximumRecursiveQuotedText);
                    recusiveIndex++;
                    const QString text = TextConverterCMark::convertMessageText(newSetting, needUpdateMessageId, recusiveIndex);
                    Utils::QuotedRichTextInfo info;
                    info.url = url;
                    info.richText = text;
                    info.displayTime = msg->dateTime();
                    quotedMessage = Utils::formatQuotedRichText(std::move(info));
                    str = str.left(startPos - 3) + str.mid(endPos + 1);
                } else {
                    qCDebug(RUQOLA_TEXTTOHTML_LOG) << "Quoted message" << messageId << "not found"; // could be a very old one
                    needUpdateMessageId = messageId;
                }
            }
        }
    }

    QString richText;
    QTextStream richTextStream(&richText);
    const QColor codeBackgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::AlternateBackground).color();
    const auto codeBorderColor = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::InactiveText).color().name();

    QString highlighted;
    QTextStream stream(&highlighted);
    TextHighlighter highlighter(&stream);
    const auto useHighlighter = SyntaxHighlightingManager::self()->syntaxHighlightingInitialized();

    if (useHighlighter) {
        auto &repo = SyntaxHighlightingManager::self()->repo();
        const auto theme = (codeBackgroundColor.lightness() < 128) ? repo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                   : repo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme);
        // qDebug() << " theme .n am" << theme.name();
        highlighter.setTheme(theme);
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
        richTextStream << "<table><tr><td style='background-color:"_L1 << codeBackgroundColor.name() << "; padding: 5px; border: 1px solid "_L1
                       << codeBorderColor << "'>"_L1 << highlight(chunk) << "</td></tr></table>"_L1;
    };

    auto addInlineCodeChunk = [&](const QString &chunk) {
        richTextStream << "<code style='background-color:"_L1 << codeBackgroundColor.name() << "'>"_L1 << chunk.toHtmlEscaped() << "</code>"_L1;
    };

    auto addTextChunk = [&](const QString &chunk) {
        auto htmlChunk = generateRichText(chunk, settings.userName, settings.highlightWords, settings.mentions, settings.channels, settings.searchedText);
        if (settings.emojiManager) {
            settings.emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << htmlChunk;
    };
    auto addInlineQuoteCodeChunk = [&](const QString &chunk) {
        auto htmlChunk = generateRichText(chunk, settings.userName, settings.highlightWords, settings.mentions, settings.channels, settings.searchedText);
        if (settings.emojiManager) {
            settings.emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << "<code style='background-color:"_L1 << codeBackgroundColor.name() << "'>"_L1 << htmlChunk << "</code>"_L1;
    };

    auto addInlineQuoteCodeNewLineChunk = [&]() {
        richTextStream << "<br />"_L1;
    };

    auto addInlineQuoteChunk = [&](const QString &chunk) {
        iterateOverEndLineRegions(chunk, QStringLiteral(">"), addInlineQuoteCodeChunk, addTextChunk, addInlineQuoteCodeNewLineChunk);
    };
    auto addNonCodeChunk = [&](QString chunk) {
        chunk = chunk.trimmed();
        if (chunk.isEmpty()) {
            return;
        }

        richTextStream << "<div>"_L1;
        iterateOverRegions(chunk, QStringLiteral("`"), addInlineCodeChunk, addInlineQuoteChunk);
        richTextStream << "</div>"_L1;
    };

    iterateOverRegions(str, QStringLiteral("```"), addCodeChunk, addNonCodeChunk);

    return "<qt>"_L1 + quotedMessage + richText + "</qt>"_L1;
}
