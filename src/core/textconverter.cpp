/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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
#include "utils.h"
#include "ruqola_debug.h"
#include "emoticons/emojimanager.h"
#include "messages/message.h"

#include "texthighlighter.h"
#include "syntaxhighlightingmanager.h"

#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>
#include <KSyntaxHighlighting/Definition>

#include <KColorScheme>

namespace
{
template<typename InRegionCallback, typename OutsideRegionCallback>
void iterateOverRegions(const QString &str, const QString &regionMarker,
                        InRegionCallback &&inRegion, OutsideRegionCallback &&outsideRegion)
{
    int startFrom = 0;
    const auto markerSize = regionMarker.size();
    while (true) {
        const int startIndex = str.indexOf(regionMarker, startFrom);
        if (startIndex == -1) {
            break;
        }
        const int endIndex = str.indexOf(regionMarker, startIndex + markerSize);
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
}

TextConverter::TextConverter(EmojiManager *emojiManager)
    : mEmojiManager(emojiManager)
{
    (void)SyntaxHighlightingManager::self();
}

QString TextConverter::convertMessageText(const QString &_str, const QString &userName, const QVector<Message> &allMessages, const QStringList &highlightWords) const
{
    if (!mEmojiManager) {
        qCWarning(RUQOLA_LOG) << "Emojimanager is null";
    }

    QString quotedMessage;

    QString str = _str;
    //TODO we need to look at room name too as we can have it when we use "direct reply"
    if (str.startsWith(QLatin1String("[ ](http"))) { // ## is there a better way?
        const int startPos = str.indexOf(QLatin1Char('('));
        const int endPos = str.indexOf(QLatin1Char(')'));
        const QString url = str.mid(startPos + 1, endPos - startPos - 1);
        // URL example https://HOSTNAME/channel/all?msg=3BR34NSG5x7ZfBa22
        // Note that this code ignores the channel name, it's always the current one...
        const QString messageId = url.mid(url.indexOf(QLatin1String("msg=")) + 4);
        //qCDebug(RUQOLA_LOG) << "Extracted messageId" << messageId;
        auto it = std::find_if(allMessages.cbegin(), allMessages.cend(), [messageId](const Message &msg) {
            return msg.messageId() == messageId;
        });
        if (it != allMessages.cend()) {
            const QString text = convertMessageText((*it).text(), userName, allMessages, highlightWords);
            quotedMessage = Utils::formatQuotedRichText(text);
            str = str.mid(endPos + 1);
        } else {
            qCDebug(RUQOLA_LOG) << "Quoted message" << messageId << "not found"; // could be a very old one
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
        highlighter.setDefinition(SyntaxHighlightingManager::self()->def());
        auto &repo = SyntaxHighlightingManager::self()->repo();
        highlighter.setTheme(codeBackgroundColor.lightness() < 128
                                            ? repo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
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

    auto addCodeChunk = [&](const QString &chunk) {
                            // Qt's support for borders is limited to tables, so we have to jump through some hoops...
                            richTextStream << QLatin1String("<table><tr><td style='background-color:") << codeBackgroundColor.name()
                                           << QLatin1String("; padding: 5px; border: 1px solid ") << codeBorderColor
                                           << QLatin1String("'>")
                                           << highlight(chunk)
                                           << QLatin1String("</td></tr></table>");
                        };

    auto addInlineCodeChunk = [&](const QString &chunk) {
        richTextStream << QLatin1String("<code style='background-color:") << codeBackgroundColor.name()
                       << QLatin1String("'>") << chunk << QLatin1String("</code>");
    };

    auto addTextChunk = [&](const QString &chunk) {
        auto htmlChunk = Utils::generateRichText(chunk, userName, highlightWords);
        if (mEmojiManager) {
            mEmojiManager->replaceEmojis(&htmlChunk);
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
