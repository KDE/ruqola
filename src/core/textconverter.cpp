/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textconverter.h"
#include "config-ruqola.h"
#if HAVE_TEXTUTILS_CMARK_SUPPORT
#include "ruqolablockcmarksupport.h"
#else
#include "cmark-rc.h"
#endif
#include "colorsandmessageviewstyle.h"
#include "config-ruqola.h"
#include "emoticons/emojimanager.h"
#include "messagecache.h"
#include "ruqola_texttohtml_cmark_debug.h"
#include "ruqola_texttohtml_debug.h"
#include "utils.h"

#include "ktexttohtmlfork/ruqolaktexttohtml.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>
#include <TextUtils/TextUtilsSyntaxHighlighter>
#include <TextUtils/TextUtilsSyntaxHighlightingManager>

#include <KColorScheme>
using namespace Qt::Literals::StringLiterals;
namespace
{
#if !HAVE_TEXTUTILS_CMARK_SUPPORT
/// check if the @p str contains an uneven number of backslashes before @p pos
bool isEscaped(const QString &str, int pos)
{
    int backslashes = 0;
    while (pos > 0 && str[pos - 1] == u'\\') {
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

template<typename InRegionCallback, typename OutsideRegionCallback, typename NewLineCallBack>
void iterateOverEndLineRegions(const QString &str,
                               const QString &regionMarker,
                               InRegionCallback &&inRegion,
                               OutsideRegionCallback &&outsideRegion,
                               NewLineCallBack &&newLine)
{
    // We have quote text if text start with > or we have "\n>"
    if (str.startsWith(regionMarker) || str.contains(u"\n"_s + regionMarker)) {
        int startFrom = 0;
        const auto markerSize = regionMarker.size();
        bool hasCode = false;
        while (true) {
            const int startIndex = findNonEscaped(str, regionMarker, startFrom);
            if (startIndex == -1) {
                break;
            }

            const int endIndex = findNewLineOrEndLine(str, u"\n"_s, startIndex + markerSize);
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
#endif
}

namespace
{
#if !HAVE_TEXTUTILS_CMARK_SUPPORT
QString markdownToRichTextCMark(const QString &markDown)
{
    if (markDown.isEmpty()) {
        return {};
    }

    qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << "BEFORE markdownToRichText " << markDown;
    QString str = markDown;

    const RuqolaKTextToHTML::Options convertFlags = RuqolaKTextToHTML::HighlightText | RuqolaKTextToHTML::ConvertPhoneNumbers;
    str = RuqolaKTextToHTML::convertToHtml(str, convertFlags);
    qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << " AFTER convertToHtml " << str;
    // substitute "[example.com](<a href="...">...</a>)" style urls
    str = Utils::convertTextWithUrl(str);
    // Substiture "- [ ] foo" and "- [x] foo" to checkmark
    str = Utils::convertTextWithCheckMark(str);
    qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << " AFTER convertTextWithUrl " << str;

    return str;
}

QString generateRichTextCMark(const QString &str,
                              const QString &username,
                              const QStringList &highlightWords,
                              const QMap<QString, QByteArray> &mentions,
                              const Channels *const channels,
                              const QString &searchedText)
{
    QString newStr = markdownToRichTextCMark(str);
    static const QRegularExpression regularExpressionAHref(u"(<a href=\'.*\'>|<a href=\".*\">)"_s);
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

        static const QRegularExpression regularExpressionRoom(u"(^|\\s+)#([\\w._-]+)"_s, QRegularExpression::UseUnicodePropertiesOption);
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

            QString wordName = word.toString();
            QByteArray roomIdentifier;
            if (channels) {
                auto it = std::find_if(channels->channels().cbegin(), channels->channels().cend(), [wordName](const auto &channel) {
                    return channel.name == wordName;
                });
                if (it == channels->channels().cend()) {
                    roomIdentifier = wordName.toLatin1();
                } else {
                    roomIdentifier = (*it).identifier;
                    if (!(*it).fname.isEmpty()) {
                        wordName = (*it).fname;
                    }
                }
            } else {
                roomIdentifier = wordName.toLatin1();
            }
            newStr.replace(u'#' + word.toString(), u"<a href=\'ruqola:/room/%2\'>#%1</a>"_s.arg(wordName, QString::fromLatin1(roomIdentifier)));
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
            const QRegularExpression exp(u"(\\b%1\\b)"_s.arg(word), QRegularExpression::CaseInsensitiveOption);
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
                    u"<a style=\"color:%2;background-color:%3;\">%1</a>"_s.arg(word, userHighlightForegroundColor, userHighlightBackgroundColor);
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

        const QRegularExpression exp(u"(%1)"_s.arg(searchedText), QRegularExpression::CaseInsensitiveOption);
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
                u"<a style=\"color:%2;background-color:%3;\">%1</a>"_s.arg(word, userHighlightForegroundColor, userHighlightBackgroundColor);
            newStr.replace(matchCapturedStart + offset, word.length(), replaceStr);
            // We added a new string => increase offset
            offset += replaceStr.length() - word.length();
        }
    }
    static const QRegularExpression regularExpressionUser(u"(^|\\s+)@([\\w._-]+)"_s, QRegularExpression::UseUnicodePropertiesOption);
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
        const int capturedStart = match.capturedStart(2) - 1;
        const int replaceWordLength = word.toString().length() + 1;
        if (word == username) {
            newStr.replace(capturedStart,
                           replaceWordLength,
                           u"<a href=\'ruqola:/user/%4\' style=\"color:%2;background-color:%3;font-weight:bold\">@%1</a>"_s.arg(word.toString(),
                                                                                                                                userMentionForegroundColor,
                                                                                                                                userMentionBackgroundColor,
                                                                                                                                wordFromUserIdentifier));

        } else {
            if (!Utils::validUser(wordFromUserIdentifier)) { // here ? all ?
                newStr.replace(capturedStart,
                               replaceWordLength,
                               u"<a style=\"color:%2;background-color:%3;font-weight:bold\">%1</a>"_s.arg(word.toString(),
                                                                                                          hereAllMentionForegroundColor,
                                                                                                          hereAllMentionBackgroundColor));
            } else {
                newStr.replace(capturedStart, replaceWordLength, u"<a href=\'ruqola:/user/%2\'>@%1</a>"_s.arg(word, wordFromUserIdentifier));
            }
        }
        userIterator = regularExpressionUser.globalMatch(newStr);
    }

    return newStr;
}

template<typename InRegionCallback, typename OutsideRegionCallback>
void iterateOverRegionsCmark(const QString &str, const QString &regionMarker, InRegionCallback &&inRegion, OutsideRegionCallback &&outsideRegion)
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

        const int adjust = str.at(endIndex - 1) == u'\n' ? 1 : 0;
        const auto codeBlock = str.mid(startIndex + markerSize, endIndex - startIndex - markerSize - adjust);

        outsideRegion(str.mid(startFrom, startIndex - startFrom));
        startFrom = endIndex + markerSize;

        inRegion(codeBlock);
    }
    outsideRegion(str.mid(startFrom));
}
#endif
}

#if !HAVE_TEXTUTILS_CMARK_SUPPORT
static QString addHighlighter(const QString &str, const TextConverter::ConvertMessageTextSettings &settings, int &blockCodeIndex, const QString &language = {})
{
    QString richText;
    QTextStream richTextStream(&richText);
    const QColor codeBackgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::AlternateBackground).color();
    const auto codeBorderColor = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::InactiveText).color().name();

    QString highlighted;
    QTextStream stream(&highlighted);
    TextUtils::TextUtilsSyntaxHighlighter highlighter(&stream);
    const auto useHighlighter = TextUtils::TextUtilsSyntaxHighlightingManager::self()->syntaxHighlightingInitialized();

    if (useHighlighter) {
        auto &repo = TextUtils::TextUtilsSyntaxHighlightingManager::self()->repo();
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
        highlighter.highlight(codeBlock, settings.messageId, blockCodeIndex);
        return highlighted;
    };

    auto addCodeChunk = [&](const QString &chunk) {
        auto definition = TextUtils::TextUtilsSyntaxHighlightingManager::self()->def(language);
        if (!definition.isValid()) {
            definition = TextUtils::TextUtilsSyntaxHighlightingManager::self()->defaultDef();
        }
        highlighter.setDefinition(definition);
        // Qt's support for borders is limited to tables, so we have to jump through some hoops...
        richTextStream << "<table><tr><td style='background-color:"_L1 << codeBackgroundColor.name() << "; padding: 5px; border: 1px solid "_L1
                       << codeBorderColor << "'>"_L1 << highlight(chunk) << "</td></tr></table>"_L1;
    };

    auto addInlineCodeChunk = [&](const QString &chunk) {
        richTextStream << "<code style='background-color:"_L1 << codeBackgroundColor.name() << "'>"_L1 << chunk.toHtmlEscaped() << "</code>"_L1;
    };

    auto addTextChunk = [&](const QString &chunk) {
        auto htmlChunk = generateRichTextCMark(chunk, settings.userName, settings.highlightWords, settings.mentions, settings.channels, settings.searchedText);
        if (settings.emojiManager) {
            settings.emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << htmlChunk;
    };
    auto addInlineQuoteCodeChunk = [&](const QString &chunk) {
        auto htmlChunk = generateRichTextCMark(chunk, settings.userName, settings.highlightWords, settings.mentions, settings.channels, settings.searchedText);
        if (settings.emojiManager) {
            settings.emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << "<code style='background-color:"_L1 << codeBackgroundColor.name() << "'>"_L1 << htmlChunk << "</code>"_L1;
    };

    auto addInlineQuoteCodeNewLineChunk = [&]() {
        richTextStream << "<br />"_L1;
    };

    auto addInlineQuoteChunk = [&](const QString &chunk) {
        iterateOverEndLineRegions(chunk, u">"_s, addInlineQuoteCodeChunk, addTextChunk, addInlineQuoteCodeNewLineChunk);
    };
    auto addNonCodeChunk = [&](const QString &chunk) {
        if (chunk.isEmpty()) {
            return;
        }

        iterateOverRegionsCmark(chunk, u"`"_s, addInlineCodeChunk, addInlineQuoteChunk);
    };

    iterateOverRegionsCmark(str, u"```"_s, addCodeChunk, addNonCodeChunk);

    qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << " richText generated: " << richText;
    return richText;
}

// #define DEBUG_CMARK_RC

static void convertHtmlChar(QString &str)
{
    str.replace(u"&gt;"_s, u">"_s);
    str.replace(u"&lt;"_s, u"<"_s);
    str.replace(u"&quot;"_s, u"\""_s);
    str.replace(u"&amp;"_s, u"&"_s);
}

static QString convertMessageText2(const TextConverter::ConvertMessageTextSettings &settings, int &numberOfTextSearched, int hightLightStringIndex)
{
    int blockCodeIndex = 1;
    // Need to escaped text (avoid to interpret html code)
    const QByteArray ba = settings.str.toUtf8();
    cmark_node *doc = cmark_parse_document(ba.constData(), ba.length(), CMARK_OPT_DEFAULT);
    cmark_iter *iter = cmark_iter_new(doc);
#ifdef DEBUG_CMARK_RC
    qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << " quotedMessage + newSettings.str.toHtmlEscaped() " << quotedMessage + newSettings.str.toHtmlEscaped();
    char *beforehtml = cmark_render_html(doc, CMARK_OPT_DEFAULT | CMARK_OPT_UNSAFE | CMARK_OPT_HARDBREAKS);
    qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << " beforehtml " << beforehtml;
    delete beforehtml;
#endif

    qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << " ba " << ba;

    while (cmark_iter_next(iter) != CMARK_EVENT_DONE) {
        cmark_node *node = cmark_iter_get_node(iter);
        qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << "type element " << cmark_node_get_type_string(node);
        switch (cmark_node_get_type(node)) {
        case CMARK_NODE_CODE_BLOCK: {
            const char *literal = cmark_node_get_literal(node);
            QString str = QString::fromUtf8(literal);
            if (!str.isEmpty()) {
                convertHtmlChar(str);
                QString language;
                const auto l = cmark_node_get_fence_info(node);
                if (l) {
                    language = QString::fromUtf8(l);
                }
                qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << " language " << language;
                const QString stringHtml = u"```"_s + str + u"```"_s;
                // qDebug() << " stringHtml " << stringHtml;
                const QString highligherStr = addHighlighter(stringHtml, settings, blockCodeIndex, language);
                cmark_node *p = cmark_node_new(CMARK_NODE_PARAGRAPH);

                cmark_node *htmlInline = cmark_node_new(CMARK_NODE_HTML_INLINE);
                cmark_node_set_literal(htmlInline, highligherStr.toUtf8().constData());
                cmark_node_append_child(p, htmlInline);

                cmark_node_replace(node, p);
            }
            break;
        }
        case CMARK_NODE_TEXT: {
            const char *literal = cmark_node_get_literal(node);
            // qDebug() << " literal" << literal;
            qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << "CMARK_NODE_TEXT: QString::fromUtf8(literal) " << QString::fromUtf8(literal);

            const QString str = QString::fromUtf8(literal);
            if (!str.isEmpty()) {
                const QString convertedString = addHighlighter(str, settings, blockCodeIndex);
                qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << "CMARK_NODE_TEXT: convert text " << convertedString;
                cmark_node *htmlInline = cmark_node_new(CMARK_NODE_HTML_INLINE);
                cmark_node_set_literal(htmlInline, convertedString.toUtf8().constData());

                cmark_node_replace(node, htmlInline);
            }
            break;
        }
        case CMARK_NODE_CODE: {
            const char *literal = cmark_node_get_literal(node);
            qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << "CMARK_NODE_CODE:  QString::fromUtf8(literal) code" << QString::fromUtf8(literal);
            QString str = QString::fromUtf8(literal);
            if (!str.isEmpty()) {
                convertHtmlChar(str);
                const QString stringHtml = u"`"_s + str + u"`"_s;
                const QString convertedString = addHighlighter(stringHtml, settings, blockCodeIndex);
                qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << "CMARK_NODE_CODE:  convert text " << convertedString;
                cmark_node *htmlInline = cmark_node_new(CMARK_NODE_HTML_INLINE);
                cmark_node_set_literal(htmlInline, convertedString.toUtf8().constData());

                cmark_node_replace(node, htmlInline);
            }
            break;
        }
        default:
            break;
        }
    }

    char *html = cmark_render_html(doc, CMARK_OPT_DEFAULT | CMARK_OPT_UNSAFE | CMARK_OPT_HARDBREAKS);
    qCDebug(RUQOLA_TEXTTOHTML_CMARK_LOG) << " generated html: " << html;

    cmark_iter_free(iter);
    cmark_node_free(doc);
    const QString result = QString::fromUtf8(html);

    cmark_mem *allocator = cmark_get_default_mem_allocator();
    allocator->free(html);

    return result;
}
#else
static QString convertMessageText2(TextConverter::ConvertMessageTextSettings *settings, int &numberOfTextSearched, int hightLightStringIndex)
{
    RuqolaBlockCMarkSupport cmarkSupport;
    cmarkSupport.setSettings(settings);
    const QString result =
        cmarkSupport.convertMessageText(settings->str, settings->messageId, settings->searchedText, numberOfTextSearched, hightLightStringIndex);
    delete settings;
    return result;
}
#endif

QString TextConverter::convertMessageText(const TextConverter::ConvertMessageTextSettings &settings,
                                          QByteArray &needUpdateMessageId,
                                          int &recusiveIndex,
                                          int &numberOfTextSearched,
                                          int hightLightStringIndex)
{
    if (!settings.emojiManager) {
        qCWarning(RUQOLA_TEXTTOHTML_LOG) << "Emojimanager is null";
    }

    QString quotedMessage;

    QString str = settings.str;
    // TODO we need to look at room name too as we can have it when we use "direct reply"
    if (str.contains("[ ](http"_L1)
        && (settings.maximumRecursiveQuotedText == -1 || (settings.maximumRecursiveQuotedText > recusiveIndex))) { // ## is there a better way?
        const int startPos = str.indexOf(u'(');
        const int endPos = str.indexOf(u')');
        const QString url = str.mid(startPos + 1, endPos - startPos - 1);
        // URL example https://HOSTNAME/channel/all?msg=3BR34NSG5x7ZfBa22
        const QByteArray messageId = url.mid(url.indexOf("msg="_L1) + 4).toLatin1();
        // qCDebug(RUQOLA_TEXTTOHTML_LOG) << "Extracted messageId" << messageId;
        auto it = std::find_if(settings.allMessages.cbegin(), settings.allMessages.cend(), [messageId](const Message &msg) {
            return msg.messageId() == messageId;
        });
        if (it != settings.allMessages.cend()) {
            const TextConverter::ConvertMessageTextSettings newSetting(u'@' + (*it).username() + u": "_s + (*it).text(),
                                                                       settings.userName,
                                                                       settings.allMessages,
                                                                       settings.highlightWords,
                                                                       settings.emojiManager,
                                                                       settings.messageCache,
                                                                       (*it).mentions(),
                                                                       (*it).channels(),
                                                                       (*it).messageId(),
                                                                       settings.searchedText,
                                                                       settings.maximumRecursiveQuotedText);
            recusiveIndex++;
            const QString text = TextConverter::convertMessageText(newSetting, needUpdateMessageId, recusiveIndex, numberOfTextSearched, hightLightStringIndex);
            Utils::QuotedRichTextInfo info;
            info.url = url;
            info.richText = text;
            info.displayTime = (*it).dateTime();

            str = str.left(startPos - 3) + str.mid(endPos + 1);
#if HAVE_TEXTUTILS_CMARK_SUPPORT
            auto newsettings = new TextConverter::ConvertMessageTextSettings{
                str.toHtmlEscaped(),
                settings.userName,
                settings.allMessages,
                settings.highlightWords,
                settings.emojiManager,
                settings.messageCache,
                settings.mentions,
                settings.channels,
                (*it).messageId(),
                settings.searchedText,
                settings.maximumRecursiveQuotedText,
            };
#else
            const TextConverter::ConvertMessageTextSettings newsettings{
                str.toHtmlEscaped(),
                settings.userName,
                settings.allMessages,
                settings.highlightWords,
                settings.emojiManager,
                settings.messageCache,
                settings.mentions,
                settings.channels,
                (*it).messageId(),
                settings.searchedText,
                settings.maximumRecursiveQuotedText,
            };

#endif
            str = convertMessageText2(newsettings, numberOfTextSearched, hightLightStringIndex);
            quotedMessage = Utils::formatQuotedRichText(info) + str;
            str.clear();
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
                                                                               msg->messageId(),
                                                                               settings.searchedText,
                                                                               settings.maximumRecursiveQuotedText);
                    recusiveIndex++;
                    const QString text =
                        TextConverter::convertMessageText(newSetting, needUpdateMessageId, recusiveIndex, numberOfTextSearched, hightLightStringIndex);
                    Utils::QuotedRichTextInfo info;
                    info.url = url;
                    info.richText = text;
                    info.displayTime = msg->dateTime();
#if HAVE_TEXTUTILS_CMARK_SUPPORT
                    auto newsettings = new TextConverter::ConvertMessageTextSettings{
                        str.toHtmlEscaped(),
                        settings.userName,
                        settings.allMessages,
                        settings.highlightWords,
                        settings.emojiManager,
                        settings.messageCache,
                        settings.mentions,
                        settings.channels,
                        msg->messageId(),
                        settings.searchedText,
                        settings.maximumRecursiveQuotedText,
                    };
#else
                    const TextConverter::ConvertMessageTextSettings newsettings{
                        str.toHtmlEscaped(),
                        settings.userName,
                        settings.allMessages,
                        settings.highlightWords,
                        settings.emojiManager,
                        settings.messageCache,
                        settings.mentions,
                        settings.channels,
                        msg->messageId(),
                        settings.searchedText,
                        settings.maximumRecursiveQuotedText,
                    };
#endif
                    str = convertMessageText2(newsettings, numberOfTextSearched, hightLightStringIndex);

                    quotedMessage = Utils::formatQuotedRichText(info) + str;
                    str.clear();
                } else {
                    qCDebug(RUQOLA_TEXTTOHTML_LOG) << "Quoted message" << messageId << "not found"; // could be a very old one
                    needUpdateMessageId = messageId;
                }
            }
        }
    }

    // Need to escaped text (avoid to interpret html code)
#if HAVE_TEXTUTILS_CMARK_SUPPORT
    auto newsettings = new TextConverter::ConvertMessageTextSettings{
        quotedMessage + str.toHtmlEscaped(),
        settings.userName,
        settings.allMessages,
        settings.highlightWords,
        settings.emojiManager,
        settings.messageCache,
        settings.mentions,
        settings.channels,
        settings.messageId,
        settings.searchedText,
        settings.maximumRecursiveQuotedText,
    };
#else
    const TextConverter::ConvertMessageTextSettings newsettings{
        quotedMessage + str.toHtmlEscaped(),
        settings.userName,
        settings.allMessages,
        settings.highlightWords,
        settings.emojiManager,
        settings.messageCache,
        settings.mentions,
        settings.channels,
        settings.messageId,
        settings.searchedText,
        settings.maximumRecursiveQuotedText,
    };
#endif

    // qDebug() << "settings.str  " << settings.str;
    const QString result = convertMessageText2(newsettings, numberOfTextSearched, hightLightStringIndex);
    // qDebug() << " RESULT ************ " << result;
    return "<qt>"_L1 + result + "</qt>"_L1;
}
