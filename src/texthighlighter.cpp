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

#include "texthighlighter.h"

#include <KSyntaxHighlighting/Format>
#include <KSyntaxHighlighting/State>
#include <KSyntaxHighlighting/Theme>

#include <QTextStream>

TextHighlighter::TextHighlighter(QTextStream *stream)
    : mStream(stream)
{
}

void TextHighlighter::highlight(const QString &str)
{
    *mStream << QStringLiteral("<pre>");

    KSyntaxHighlighting::State state;

    int lineStart = 0;
    int lineEnd = str.indexOf(QLatin1Char('\n'));

    for (; lineEnd != -1; lineStart = lineEnd + 1, lineEnd = str.indexOf(QLatin1Char('\n'), lineStart)) {
        mCurrentLine = str.mid(lineStart, lineEnd - lineStart);
        state = highlightLine(mCurrentLine, state);
        //*mStream << QLatin1Char('\n');
        *mStream << QStringLiteral("<br>");
    }
    if (lineStart < str.size()) { // remaining content if str isn't ending with a newline
        mCurrentLine = str.mid(lineStart);
        state = highlightLine(mCurrentLine, state);
        //*mStream << QLatin1Char('\n');
        *mStream << QStringLiteral("<br>");
    }
    *mStream << QLatin1String("</pre>");
}

void TextHighlighter::applyFormat(int offset, int length, const KSyntaxHighlighting::Format &format)
{
    if (!format.isDefaultTextStyle(theme())) {
        *mStream << QStringLiteral("<span style=\"");
        if (format.hasTextColor(theme())) {
            *mStream << QStringLiteral("color:") << format.textColor(theme()).name() << QStringLiteral(";");
        }
        if (format.hasBackgroundColor(theme())) {
            *mStream << QStringLiteral("background-color:") << format.backgroundColor(theme()).name() << QStringLiteral(";");
        }
        if (format.isBold(theme())) {
            *mStream << QStringLiteral("font-weight:bold;");
        }
        if (format.isItalic(theme())) {
            *mStream << QStringLiteral("font-style:italic;");
        }
        if (format.isUnderline(theme())) {
            *mStream << QStringLiteral("text-decoration:underline;");
        }
        if (format.isStrikeThrough(theme())) {
            *mStream << QStringLiteral("text-decoration:line-through;");
        }
        *mStream << QStringLiteral("\">");
    }

    *mStream << mCurrentLine.mid(offset, length).toHtmlEscaped();

    if (!format.isDefaultTextStyle(theme())) {
        *mStream << QStringLiteral("</span>");
    }
}
