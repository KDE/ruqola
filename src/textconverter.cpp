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

#include "texthighlighter.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

//#define USE_SYNTAXHIGHLIGHTING 1

TextConverter::TextConverter()
{

}

QString TextConverter::convertMessageText(const QString &str, const QMap<QString, QString> &mentions) const
{
#ifdef USE_SYNTAXHIGHLIGHTING
    if (str.startsWith(QLatin1String("```")) && str.endsWith(QLatin1String("```"))) {
        QString e = str;
        e = e.remove(QLatin1String("```"));
        QString result;
        QTextStream s(&result);
        const auto def = mRepo.definitionForName(QStringLiteral("C++"));
        if (!def.isValid()) {
            qCWarning(RUQOLA_LOG) << "Unable to find definition";
            return {};
        }

        TextHighlighter highLighter(&s);
        highLighter.setDefinition(def);
        highLighter.setTheme(/*QGuiApplication::palette().color(QPalette::Base).lightness() < 128
                             ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                             : */mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme));
        highLighter.highlight(e);
        return *s.string();
    }
#endif
    return Utils::generateRichText(str, mentions);
}
