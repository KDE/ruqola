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

TextConverter::TextConverter(EmojiManager *emojiManager)
    : mEmojiManager(emojiManager)
{
    (void)SyntaxHighlightingManager::self();
}

QString TextConverter::convertMessageText(const QString &_str, const QString &userName, const QVector<Message> &allMessages) const
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
            const QString text = convertMessageText((*it).text(), userName, allMessages);
            quotedMessage = Utils::formatQuotedRichText(text);
            str = str.mid(endPos + 1);
        } else {
            qCDebug(RUQOLA_LOG) << "Quoted message" << messageId << "not found"; // could be a very old one
        }
    }

    QString richText;
    QTextStream richTextStream(&richText);
    auto addHtmlChunk = [&richTextStream](const QString &htmlChunk) {
                            richTextStream << QLatin1String("<div>") << htmlChunk << QLatin1String("</div>");
                        };
    KColorScheme scheme;
    const auto codeBackgroundColor = scheme.background(KColorScheme::AlternateBackground).color().name();
    const auto codeBorderColor = scheme.foreground(KColorScheme::InactiveText).color().name();
    auto addCodeChunk = [&](const QString &htmlChunk) {
                            // Qt's support for borders is limited to tables, so we have to jump through some hoops...
                            richTextStream << QLatin1String("<table><tr><td style='background-color:") << codeBackgroundColor
                                           << QLatin1String("; padding: 5px; border: 1px solid ") << codeBorderColor
                                           << QLatin1String("'>")
                                           << htmlChunk
                                           << QLatin1String("</td></tr></table>");
                        };
    auto addNonCodeChunk = [&](QString chunk) {
                               chunk = chunk.trimmed();
                               if (chunk.isEmpty()) {
                                   return;
                               }
                               auto htmlChunk = Utils::generateRichText(chunk, userName);
                               if (mEmojiManager) {
                                   mEmojiManager->replaceEmojis(&htmlChunk);
                               }
                               addHtmlChunk(htmlChunk);
                           };

    if (SyntaxHighlightingManager::self()->syntaxHighlightingInitialized()) {
        QString highlighted;
        QTextStream stream(&highlighted);
        TextHighlighter highLighter(&stream);
        highLighter.setDefinition(SyntaxHighlightingManager::self()->def());
        highLighter.setTheme(/*QGuiApplication::palette().color(QPalette::Base).lightness() < 128
                                            ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                            : */SyntaxHighlightingManager::self()->repo().defaultTheme(KSyntaxHighlighting::Repository::DarkTheme));

        int startFrom = 0;
        while (true) {
            const int startIndex = str.indexOf(QLatin1String("```"), startFrom);
            if (startIndex == -1) {
                break;
            }
            const int endIndex = str.indexOf(QLatin1String("```"), startIndex + 3);
            if (endIndex == -1) {
                break;
            }
            const auto codeBlock = str.mid(startIndex + 3, endIndex - startIndex - 3).trimmed();

            addNonCodeChunk(str.mid(startFrom, startIndex - startFrom));
            startFrom = endIndex + 3;

            stream.reset();
            stream.seek(0);
            highlighted.clear();
            highLighter.highlight(codeBlock);
            addCodeChunk(highlighted);
        }
        addNonCodeChunk(str.mid(startFrom));
    } else {
        addNonCodeChunk(str);
    }

    return QLatin1String("<qt>") + quotedMessage + richText + QLatin1String("</qt>");
}
