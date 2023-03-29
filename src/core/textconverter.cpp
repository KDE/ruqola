/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textconverter.h"
#include "colors.h"
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
#define SUPPORT_QUOTE_TEXT 1
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
#ifdef SUPPORT_QUOTE_TEXT
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
#endif

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

#ifdef SUPPORT_QUOTE_TEXT
template<typename InRegionCallback, typename OutsideRegionCallback, typename NewLineCallBack>
void iterateOverEndLineRegions(const QString &str,
                               const QString &regionMarker,
                               InRegionCallback &&inRegion,
                               OutsideRegionCallback &&outsideRegion,
                               NewLineCallBack &&newLine)
{
    // We have quote text if text start with > or we have "\n>"
    if (str.startsWith(regionMarker) || str.contains(QStringLiteral("\n>"))) {
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QStringView codeBlock = str.midRef(startIndex + markerSize, endIndex - startIndex).trimmed();
#else
            QStringView codeBlock = str.mid(startIndex + markerSize, endIndex - startIndex).trimmed();
#endif
            if (codeBlock.endsWith(regionMarker)) {
                codeBlock.chop(regionMarker.size());
            }
            if (hasCode) {
                newLine();
            }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            const QStringView midCode = str.midRef(startFrom, startIndex - startFrom);
#else
            const QStringView midCode = str.mid(startFrom, startIndex - startFrom);
#endif
            outsideRegion(midCode.toString());
            startFrom = endIndex + markerSize;

            inRegion(codeBlock.toString());
            if (!codeBlock.isEmpty()) {
                hasCode = true;
            }
        }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        const auto afterstr = str.midRef(startFrom);
        outsideRegion(afterstr.toString());
#else
        const auto afterstr = str.mid(startFrom);
        outsideRegion(afterstr);
#endif
    } else {
        outsideRegion(str);
    }
}
#endif

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
    qCDebug(RUQOLA_TEXTTOHTML_LOG) << " AFTER convertTextWithUrl " << str;

    return str;
}

QString generateRichText(const QString &str,
                         const QString &username,
                         const QStringList &highlightWords,
                         const QMap<QString, QString> &mentions,
                         const QMap<QString, QString> &channels,
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            const QStringRef word = match.capturedRef(2);
#else
            const QStringView word = match.capturedView(2);
#endif
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
            QString roomIdentifier = channels.value(word.toString());
            if (roomIdentifier.isEmpty()) {
                roomIdentifier = word.toString();
            }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            newStr.replace(QLatin1Char('#') + word, QStringLiteral("<a href=\'ruqola:/room/%2\'>#%1</a>").arg(word, roomIdentifier));
#else
            newStr.replace(QLatin1Char('#') + word.toString(), QStringLiteral("<a href=\'ruqola:/room/%2\'>#%1</a>").arg(word, roomIdentifier));
#endif
        }
    }

    if (!highlightWords.isEmpty()) {
        const auto userHighlightForegroundColor = Colors::self().schemeView().foreground(KColorScheme::PositiveText).color().name();
        const auto userHighlightBackgroundColor = Colors::self().schemeView().background(KColorScheme::PositiveBackground).color().name();
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
        const auto userHighlightForegroundColor = Colors::self().schemeView().foreground(KColorScheme::NeutralText).color().name();
        const auto userHighlightBackgroundColor = Colors::self().schemeView().background(KColorScheme::NeutralBackground).color().name();
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

    const auto userMentionForegroundColor = Colors::self().schemeView().foreground(KColorScheme::NegativeText).color().name();
    const auto userMentionBackgroundColor = Colors::self().schemeView().background(KColorScheme::NegativeBackground).color().name();
    while (userIterator.hasNext()) {
        const QRegularExpressionMatch match = userIterator.next();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        const QStringRef word = match.capturedRef(2);
#else
        const QStringView word = match.capturedView(2);
#endif
        // Highlight only if it's yours

        QString userIdentifier = mentions.value(word.toString());
        if (userIdentifier.isEmpty()) {
            userIdentifier = word.toString();
        }
        if (word == username) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            newStr.replace(QLatin1Char('@') + word,
                           QStringLiteral("<a href=\'ruqola:/user/%4\' style=\"color:%2;background-color:%3;font-weight:bold\">@%1</a>")
                               .arg(word.toString(), userMentionForegroundColor, userMentionBackgroundColor, userIdentifier));
#else
            newStr.replace(QLatin1Char('@') + word.toString(),
                           QStringLiteral("<a href=\'ruqola:/user/%4\' style=\"color:%2;background-color:%3;font-weight:bold\">@%1</a>")
                               .arg(word.toString(), userMentionForegroundColor, userMentionBackgroundColor, userIdentifier));

#endif
        } else {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            newStr.replace(QLatin1Char('@') + word, QStringLiteral("<a href=\'ruqola:/user/%2\'>@%1</a>").arg(word, userIdentifier));
#else
            newStr.replace(QLatin1Char('@') + word.toString(), QStringLiteral("<a href=\'ruqola:/user/%2\'>@%1</a>").arg(word, userIdentifier));
#endif
        }
    }

    return newStr;
}
}

QString TextConverter::convertMessageText(const ConvertMessageTextSettings &settings, QString &needUpdateMessageId, int &recusiveIndex)
{
    if (!settings.emojiManager) {
        qCWarning(RUQOLA_TEXTTOHTML_LOG) << "Emojimanager is null";
    }

    QString quotedMessage;

    QString str = settings.str;
    // TODO we need to look at room name too as we can have it when we use "direct reply"
    if (str.contains(QLatin1String("[ ](http"))
        && (settings.maximumRecursiveQuotedText == -1 || (settings.maximumRecursiveQuotedText > recusiveIndex))) { // ## is there a better way?
        const int startPos = str.indexOf(QLatin1Char('('));
        const int endPos = str.indexOf(QLatin1Char(')'));
        const QString url = str.mid(startPos + 1, endPos - startPos - 1);
        // URL example https://HOSTNAME/channel/all?msg=3BR34NSG5x7ZfBa22
        const QString messageId = url.mid(url.indexOf(QLatin1String("msg=")) + 4);
        // qCDebug(RUQOLA_TEXTTOHTML_LOG) << "Extracted messageId" << messageId;
        auto it = std::find_if(settings.allMessages.cbegin(), settings.allMessages.cend(), [messageId](const Message &msg) {
            return msg.messageId() == messageId;
        });
        if (it != settings.allMessages.cend()) {
            const ConvertMessageTextSettings newSetting(QLatin1Char('@') + (*it).username() + QStringLiteral(": ") + (*it).text(),
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
            const QString text = convertMessageText(newSetting, needUpdateMessageId, recusiveIndex);
            Utils::QuotedRichTextInfo info;
            info.url = url;
            info.richText = text;
            info.displayTime = (*it).dateTime();
            quotedMessage = Utils::formatQuotedRichText(info);
            str = str.left(startPos - 3) + str.mid(endPos + 1);
        } else {
            if (settings.messageCache) {
                // TODO allow to reload index when we loaded message
                Message *msg = settings.messageCache->messageForId(messageId);
                if (msg) {
                    const ConvertMessageTextSettings newSetting(msg->text(),
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
                    const QString text = convertMessageText(newSetting, needUpdateMessageId, recusiveIndex);
                    Utils::QuotedRichTextInfo info;
                    info.url = url;
                    info.richText = text;
                    info.displayTime = msg->dateTime();
                    quotedMessage = Utils::formatQuotedRichText(info);
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
    const auto codeBackgroundColor = Colors::self().schemeView().background(KColorScheme::AlternateBackground).color();
    const auto codeBorderColor = Colors::self().schemeView().foreground(KColorScheme::InactiveText).color().name();

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
        auto htmlChunk = generateRichText(chunk, settings.userName, settings.highlightWords, settings.mentions, settings.channels, settings.searchedText);
        if (settings.emojiManager) {
            settings.emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << htmlChunk;
    };
#ifdef SUPPORT_QUOTE_TEXT
    auto addInlineQuoteCodeChunk = [&](const QString &chunk) {
        auto htmlChunk = generateRichText(chunk, settings.userName, settings.highlightWords, settings.mentions, settings.channels, settings.searchedText);
        if (settings.emojiManager) {
            settings.emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << QLatin1String("<code style='background-color:") << codeBackgroundColor.name() << QLatin1String("'>") << htmlChunk
                       << QLatin1String("</code>");
    };

    auto addInlineQuoteCodeNewLineChunk = [&]() {
        richTextStream << QLatin1String("<br />");
    };

    auto addInlineQuoteChunk = [&](const QString &chunk) {
        iterateOverEndLineRegions(chunk, QStringLiteral(">"), addInlineQuoteCodeChunk, addTextChunk, addInlineQuoteCodeNewLineChunk);
    };
#endif
    auto addNonCodeChunk = [&](QString chunk) {
        chunk = chunk.trimmed();
        if (chunk.isEmpty()) {
            return;
        }

        richTextStream << QLatin1String("<div>");
#ifdef SUPPORT_QUOTE_TEXT
        iterateOverRegions(chunk, QStringLiteral("`"), addInlineCodeChunk, addInlineQuoteChunk);
#else
        iterateOverRegions(chunk, QStringLiteral("`"), addInlineCodeChunk, addTextChunk);
#endif
        richTextStream << QLatin1String("</div>");
    };

    iterateOverRegions(str, QStringLiteral("```"), addCodeChunk, addNonCodeChunk);

    return QLatin1String("<qt>") + quotedMessage + richText + QLatin1String("</qt>");
}
