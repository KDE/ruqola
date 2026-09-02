/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolablockcmarksupport.h"
#include "colorsandmessageviewstyle.h"
#include "config-ruqola.h"
#include "emoticons/emojimanager.h"
#include "ruqola_texttohtml_cmark_debug.h"
#include <KSyntaxHighlighting/Theme>
#include <QTextStream>
#include <TextUtils/TextUtilsSyntaxHighlighter>
#include <TextUtils/TextUtilsSyntaxHighlightingManager>

using namespace Qt::Literals::StringLiterals;
RuqolaBlockCMarkSupport::RuqolaBlockCMarkSupport() = default;

RuqolaBlockCMarkSupport::~RuqolaBlockCMarkSupport() = default;

namespace
{
// True when @p regionMarker starts a line of @p str, i.e. it sits at the very beginning or
// right after a newline. Equivalent to str.startsWith(m) || str.contains(u'\n' + m), but
// without allocating the concatenated marker on every call.
[[nodiscard]] bool hasMarkerAtLineStart(const QString &str, const QString &regionMarker)
{
    for (qsizetype i = str.indexOf(regionMarker); i != -1; i = str.indexOf(regionMarker, i + 1)) {
        if (i == 0 || str.at(i - 1) == u'\n') {
            return true;
        }
    }
    return false;
}

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
    // We have quote text if the marker starts the text or starts any line of it
    if (hasMarkerAtLineStart(str, regionMarker)) {
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
                              const QList<QRegularExpression> &highlightWords,
                              const QMap<QString, QByteArray> &mentions,
                              const Channels *const channels,
                              const QRegularExpression &searchedTextRegularExpression)
{
    QString newStr = markdownToRichTextCMark(str);
    static const QRegularExpression regularExpressionAHref(u"(<a href=\'.*\'>|<a href=\".*\">)"_s);
    regularExpressionAHref.optimize();
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
        const QList<Channels::ChannelInfo> channelsList = channels ? channels->channels() : QList<Channels::ChannelInfo>{};
        int offset = 0;
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
            const auto it = std::find_if(channelsList.cbegin(), channelsList.cend(), [&wordName](const auto &channel) {
                return channel.name == wordName;
            });
            if (it == channelsList.cend()) {
                roomIdentifier = wordName.toLatin1();
            } else {
                roomIdentifier = it->identifier;
                if (!it->fname.isEmpty()) {
                    wordName = it->fname;
                }
            }
            const QString replaceStr = u"<a href=\'ruqola:/room/%2\'>#%1</a>"_s.arg(wordName, QString::fromLatin1(roomIdentifier));
            // Replace at the match position: replacing by value would rewrite every other
            // occurrence of the same room, nesting the anchors we just inserted.
            const int replaceWordLength = word.length() + 1; // '#' + word
            newStr.replace(matchCapturedStart - 1 + offset, replaceWordLength, replaceStr);
            // We added a new string => increase offset
            offset += replaceStr.length() - replaceWordLength;
        }
    }

    if (!highlightWords.isEmpty()) {
        const QString &userHighlightForegroundColor = ColorsAndMessageViewStyle::self().positiveText();
        const QString &userHighlightBackgroundColor = ColorsAndMessageViewStyle::self().positiveBackground();
        lstPos.clear();
        QRegularExpressionMatchIterator userIteratorHref = regularExpressionAHref.globalMatch(newStr);
        while (userIteratorHref.hasNext()) {
            const QRegularExpressionMatch match = userIteratorHref.next();
            HrefPos pos;
            pos.start = match.capturedStart(1);
            pos.end = match.capturedEnd(1);
            lstPos.append(std::move(pos));
        }

        for (const auto &exp : highlightWords) {
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

    if (!searchedTextRegularExpression.pattern().isEmpty()) {
        const QString &userHighlightForegroundColor = ColorsAndMessageViewStyle::self().neutralText();
        const QString &userHighlightBackgroundColor = ColorsAndMessageViewStyle::self().neutralBackground();
        lstPos.clear();
        QRegularExpressionMatchIterator userIteratorHref = regularExpressionAHref.globalMatch(newStr);
        while (userIteratorHref.hasNext()) {
            const QRegularExpressionMatch match = userIteratorHref.next();
            HrefPos pos;
            pos.start = match.capturedStart(1);
            pos.end = match.capturedEnd(1);
            lstPos.append(std::move(pos));
        }

        QRegularExpressionMatchIterator userIterator = searchedTextRegularExpression.globalMatch(newStr);
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

    int offset = 0;
    while (userIterator.hasNext()) {
        const QRegularExpressionMatch match = userIterator.next();
        const QStringView word = match.capturedView(2);
        // Highlight only if it's yours

        const QString wordStr = word.toString();
        const QByteArray userIdentifier = mentions.value(wordStr);
        QString wordFromUserIdentifier = QString::fromLatin1(userIdentifier);
        if (/*userIdentifier.isEmpty()*/ 1) {
            wordFromUserIdentifier = wordStr;
        }
        const int capturedStart = match.capturedStart(2) - 1;
        const int replaceWordLength = wordStr.length() + 1; // '@' + word
        QString replaceStr;
        if (word == username) {
            replaceStr = u"<a href=\'ruqola:/user/%4\' style=\"color:%2;background-color:%3;font-weight:bold\">@%1</a>"_s.arg(
                wordStr,
                ColorsAndMessageViewStyle::self().negativeText(),
                ColorsAndMessageViewStyle::self().negativeBackground(),
                wordFromUserIdentifier);
        } else if (!Utils::validUser(wordFromUserIdentifier)) { // here ? all ?
            replaceStr = u"<a style=\"color:%2;background-color:%3;font-weight:bold\">%1</a>"_s.arg(wordStr,
                                                                                                    ColorsAndMessageViewStyle::self().neutralText(),
                                                                                                    ColorsAndMessageViewStyle::self().neutralBackground());
        } else {
            replaceStr = u"<a href=\'ruqola:/user/%2\'>@%1</a>"_s.arg(wordStr, wordFromUserIdentifier);
        }
        // Inserted anchors put the '@' behind a '>', so a single pass is enough: the text we
        // add can never match the regexp again.
        newStr.replace(capturedStart + offset, replaceWordLength, replaceStr);
        // We added a new string => increase offset
        offset += replaceStr.length() - replaceWordLength;
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
    regenerateSearchText();
    QString richText;
    QTextStream richTextStream(&richText);

    QString highlighted;
    QTextStream stream(&highlighted);
    TextUtils::TextUtilsSyntaxHighlighter highlighter(&stream);
    const auto useHighlighter = TextUtils::TextUtilsSyntaxHighlightingManager::self()->syntaxHighlightingInitialized();

    if (useHighlighter) {
        auto &repo = TextUtils::TextUtilsSyntaxHighlightingManager::self()->repo();
        const auto theme = ColorsAndMessageViewStyle::self().darkTheme() ? repo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                         : repo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme);
        // qDebug() << " theme .n am" << theme.name();
        highlighter.setTheme(theme);
    }
    auto highlight = [&](const QString &codeBlock) {
        if (!useHighlighter) {
            return codeBlock.toHtmlEscaped();
        }
        stream.reset();
        stream.seek(0);
        highlighted.clear();
        highlighter.highlight(codeBlock, mSettings->messageId, blockCodeIndex);
        return highlighted;
    };

    auto addCodeChunk = [&](const QString &chunk) {
        auto definition = TextUtils::TextUtilsSyntaxHighlightingManager::self()->def(language);
        if (!definition.isValid()) {
            definition = TextUtils::TextUtilsSyntaxHighlightingManager::self()->defaultDef();
        }

        highlighter.setDefinition(definition);
        // Qt's support for borders is limited to tables, so we have to jump through some hoops...
        richTextStream << "<table><tr><td style='background-color:"_L1 << ColorsAndMessageViewStyle::self().alternateBackground()
                       << "; padding: 5px; border: 1px solid "_L1 << ColorsAndMessageViewStyle::self().inactiveText() << "'>"_L1 << highlight(chunk)
                       << "</td></tr></table>"_L1;
    };

    auto addInlineCodeChunk = [&](const QString &chunk) {
        richTextStream << "<code style='background-color:"_L1 << ColorsAndMessageViewStyle::self().alternateBackground() << "'>"_L1 << chunk.toHtmlEscaped()
                       << "</code>"_L1;
    };

    auto addTextChunk = [&](const QString &chunk) {
        auto htmlChunk =
            generateRichTextCMark(chunk, mSettings->userName, mSettings->highlightWords, mSettings->mentions, mSettings->channels, mSearchRegularExpression);
        if (mSettings->emojiManager) {
            mSettings->emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << htmlChunk;
    };
    auto addInlineQuoteCodeChunk = [&](const QString &chunk) {
        auto htmlChunk =
            generateRichTextCMark(chunk, mSettings->userName, mSettings->highlightWords, mSettings->mentions, mSettings->channels, mSearchRegularExpression);
        if (mSettings->emojiManager) {
            mSettings->emojiManager->replaceEmojis(&htmlChunk);
        }
        richTextStream << "<code style='background-color:"_L1 << ColorsAndMessageViewStyle::self().alternateBackground() << "'>"_L1 << htmlChunk
                       << "</code>"_L1;
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

void RuqolaBlockCMarkSupport::regenerateSearchText()
{
    if (mSearchText == mSettings->searchedText) {
        return;
    }
    mSearchText = mSettings->searchedText;
    // An empty searched text must clear the pattern: escaping it would build "()", which matches
    // the empty string at every position.
    mSearchRegularExpression = mSearchText.isEmpty()
        ? QRegularExpression{}
        : QRegularExpression(u"(%1)"_s.arg(QRegularExpression::escape(mSearchText)), QRegularExpression::CaseInsensitiveOption);
}

TextConverter::ConvertMessageTextSettings *RuqolaBlockCMarkSupport::settings() const
{
    return mSettings;
}

void RuqolaBlockCMarkSupport::setSettings(TextConverter::ConvertMessageTextSettings *newSettings)
{
    mSettings = newSettings;
}
