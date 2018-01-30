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

#include "rocketchataccount.h"
#include "texthighlighter.h"

#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

TextConverter::TextConverter(RocketChatAccount *account)
    : mRocketChatAccount(account)
{
    mDef = mRepo.definitionForName(QStringLiteral("C++"));
    if (mDef.isValid()) {
        mSyntaxHighlightingInitialized = true;
    } else {
        qCWarning(RUQOLA_LOG) << "Unable to find definition";
    }
}

QString TextConverter::convertMessageText(const QString &str, const QMap<QString, QString> &mentions) const
{
    //TODO improve it. Add autotest.
    if (mSyntaxHighlightingInitialized && str.startsWith(QLatin1String("```")) && str.endsWith(QLatin1String("```"))) {
        QString e = str;
        e = e.remove(QLatin1String("```"));
        QString result;
        QTextStream s(&result);

        TextHighlighter highLighter(&s);
        highLighter.setDefinition(mDef);
        highLighter.setTheme(/*QGuiApplication::palette().color(QPalette::Base).lightness() < 128
                             ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                             : */mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme));
        highLighter.highlight(e);
        return *s.string();
    }
    QString richText = Utils::generateRichText(str, mentions);
    if (mRocketChatAccount) {
        //TODO use custom emoji
        //TODO replace custom emoji
        //mRocketChatAccount->emojiManager()->html(QString());
    }
    return richText;
}
