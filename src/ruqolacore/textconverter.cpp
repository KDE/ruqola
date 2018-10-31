/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

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
#include "emojimanager.h"

#include "texthighlighter.h"
#include "syntaxhighlightingmanager.h"

#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

#include <QRegularExpressionMatch>

TextConverter::TextConverter(EmojiManager *emojiManager)
    : mEmojiManager(emojiManager)
{
    (void)SyntaxHighlightingManager::self();
}

QString TextConverter::convertMessageText(const QString &str, const QMap<QString, QString> &mentions, const QString &userName) const
{
    if (SyntaxHighlightingManager::self()->syntaxHighlightingInitialized()) {
        const int startIndex = str.indexOf(QLatin1String("```"));
        const int endIndex = str.lastIndexOf(QLatin1String("```"));
        if ((startIndex > -1) && (endIndex > -1) && (startIndex != endIndex)) {
            QString beginStr;
            if (startIndex > 0) {
                beginStr = str.left(startIndex);
            }
            //qDebug() << " end " << endIndex << str.length();
            QString quoteStr = str.mid(startIndex + 3, endIndex - startIndex - 3);
            QString endStr = str.right(str.length() - endIndex -3 );
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
            return beginStr + *s.string() + endStr;
        }
    }
    QString richText = Utils::generateRichText(str, mentions, userName);
    if (mEmojiManager) {
        static const QRegularExpression regularExpressionUser(QStringLiteral("(:\\w+:)"));
        QRegularExpressionMatchIterator userIterator = regularExpressionUser.globalMatch(richText);
        while (userIterator.hasNext()) {
            const QRegularExpressionMatch match = userIterator.next();
            const QString word = match.captured(1);
            const QString replaceWord = mEmojiManager->html(word);
            if (!replaceWord.isEmpty()) {
                richText.replace(word, replaceWord);
            }
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Emojimanager was not setted";
    }
    return richText;
}
