/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolablockcmarksupport.h"
#include "colorsandmessageviewstyle.h"
#include "config-ruqola.h"
#include "emoticons/emojimanager.h"
#include "ruqola_texttohtml_cmark_debug.h"
#include <KColorScheme>
#include <KSyntaxHighlighting/Theme>
#include <QColor>
#include <QTextStream>
#include <TextUtils/TextUtilsSyntaxHighlighter>
#include <TextUtils/TextUtilsSyntaxHighlightingManager>

using namespace Qt::Literals::StringLiterals;
RuqolaBlockCMarkSupport::RuqolaBlockCMarkSupport() = default;

RuqolaBlockCMarkSupport::~RuqolaBlockCMarkSupport() = default;

namespace
{
template<typename InRegionCallback, typename OutsideRegionCallback>
void iterateOverRegionsCmark(const QString &str, const QString &regionMarker, InRegionCallback &&inRegion, OutsideRegionCallback &&outsideRegion)
{
    int startFrom = 0;
    const auto markerSize = regionMarker.size();
    while (true) {
        const int startIndex = TextUtils::TextUtilsBlockCMarkSupport::findNonEscaped(str, regionMarker, startFrom);
        if (startIndex == -1) {
            break;
        }

        const int endIndex = TextUtils::TextUtilsBlockCMarkSupport::findNonEscaped(str, regionMarker, startIndex + markerSize);
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
            const int startIndex = TextUtils::TextUtilsBlockCMarkSupport::findNonEscaped(str, regionMarker, startFrom);
            if (startIndex == -1) {
                break;
            }

            const int endIndex = TextUtils::TextUtilsBlockCMarkSupport::findNewLineOrEndLine(str, u"\n"_s, startIndex + markerSize);
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

QString markdownToRichTextCMark(const QString &markDown)
{
    if (markDown.isEmpty()) {
        return {};
    }

    QString str = TextUtils::TextUtilsBlockCMarkSupport::markdownToRichTextCMark(markDown);
    str = Utils::convertTextWithCheckMark(str);
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
}

QString RuqolaBlockCMarkSupport::addHighlighter(const QString &str,
                                                const QString &language,
                                                [[maybe_unused]] const QString &searchText,
                                                [[maybe_unused]] const QByteArray &uuid,
                                                int &blockCodeIndex,
                                                int &numberOfTextSearched,
                                                int hightLightStringIndex)
{
    if (!mSettings) {
        qCWarning(RUQOLA_TEXTTOHTML_CMARK_LOG) << " TextConverter::ConvertMessageTextSettings is null. IT's a bug";
        return {};
    }
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
#if HAVE_TEXTUTILS_SYNTAXHIGHLIGTHER_SUPPORT
        highlighter.highlight(codeBlock, mSettings->messageId, blockCodeIndex);
#else
        highlighter.highlight(codeBlock);
#endif
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
        auto htmlChunk =
            generateRichTextCMark(chunk, mSettings->userName, mSettings->highlightWords, mSettings->mentions, mSettings->channels, mSettings->searchedText);
        if (mSettings->emojiManager) {
            mSettings->emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << htmlChunk;
    };
    auto addInlineQuoteCodeChunk = [&](const QString &chunk) {
        auto htmlChunk =
            generateRichTextCMark(chunk, mSettings->userName, mSettings->highlightWords, mSettings->mentions, mSettings->channels, mSettings->searchedText);
        if (mSettings->emojiManager) {
            mSettings->emojiManager->replaceEmojis(&htmlChunk);
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

TextConverter::ConvertMessageTextSettings *RuqolaBlockCMarkSupport::settings() const
{
    return mSettings;
}

void RuqolaBlockCMarkSupport::setSettings(TextConverter::ConvertMessageTextSettings *newSettings)
{
    mSettings = newSettings;
}
