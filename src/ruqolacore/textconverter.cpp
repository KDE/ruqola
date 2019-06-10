/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include <QRegularExpressionMatch>

TextConverter::TextConverter(EmojiManager *emojiManager)
    : mEmojiManager(emojiManager)
{
    (void)SyntaxHighlightingManager::self();
}

QString TextConverter::convertMessageText(const QString &_str, const QString &userName, const QVector<Message> &allMessages, const QString &threadMessageId) const
{
    QString str = _str;
    QString quotedMessage;
    QString quotedThreadMessage;
    if (!threadMessageId.isEmpty()) {
        auto it = std::find_if(allMessages.cbegin(), allMessages.cend(), [threadMessageId](const Message &msg) {
            return msg.messageId() == threadMessageId;
        });
        if (it != allMessages.cend()) {
            //Fix color
            quotedThreadMessage = QStringLiteral("<font color=\"red\" size=\"-1\">%1</font><br/>").arg((*it).text());
        } else {
            qCDebug(RUQOLA_LOG) << "Thread message" << threadMessageId << "not found"; // could be a very old one
        }

    }
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
            quotedMessage = QStringLiteral("<font size=\"-1\">&gt; %1</font><br/>").arg((*it).text());
            str = str.mid(endPos + 1);
        } else {
            qCDebug(RUQOLA_LOG) << "Quoted message" << messageId << "not found"; // could be a very old one
        }
    }
    if (SyntaxHighlightingManager::self()->syntaxHighlightingInitialized()) {
        const int startIndex = str.indexOf(QLatin1String("```"));
        const int endIndex = str.lastIndexOf(QLatin1String("```"));
        if ((startIndex > -1) && (endIndex > -1) && (startIndex != endIndex)) {
            QString beginStr;
            if (startIndex > 0) {
                beginStr = str.left(startIndex);
            }
            //qDebug() << " end " << endIndex << str.length();
            const QString quoteStr = str.mid(startIndex + 3, endIndex - startIndex - 3);
            const QString endStr = str.right(str.length() - endIndex -3);
            QString result;
//            qDebug() << " beginStr" << beginStr;
//            qDebug() << " endStr" << endStr;
//            qDebug() << " quoteStr" << quoteStr;
//            qDebug() << " str " << str;
            QTextStream s(&result);

            TextHighlighter highLighter(&s);
            highLighter.setDefinition(SyntaxHighlightingManager::self()->def());
            highLighter.setTheme(/*QGuiApplication::palette().color(QPalette::Base).lightness() < 128
                                                  ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                  : */SyntaxHighlightingManager::self()->repo().defaultTheme(KSyntaxHighlighting::Repository::DarkTheme));
            highLighter.highlight(quoteStr);
            return quotedThreadMessage + quotedMessage + beginStr + *s.string() + endStr;
        }
    }
    QString richText = Utils::generateRichText(str, userName);
    if (mEmojiManager) {
        static const QRegularExpression regularExpressionUser(QStringLiteral("(:\\w+:)"));
        QRegularExpressionMatchIterator userIterator = regularExpressionUser.globalMatch(richText);
        while (userIterator.hasNext()) {
            const QRegularExpressionMatch match = userIterator.next();
            const QString word = match.captured(1);
            const QString replaceWord = mEmojiManager->replaceEmojiIdentifier(word);
            if (!replaceWord.isEmpty()) {
                richText.replace(word, replaceWord);
            }
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Emojimanager was not setted";
    }
    return quotedThreadMessage + quotedMessage + richText;
}
