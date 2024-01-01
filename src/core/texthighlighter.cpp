/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    *mStream << QStringLiteral("<code>");

    KSyntaxHighlighting::State state;

    int lineStart = 0;
    int lineEnd = str.indexOf(QLatin1Char('\n'));

    for (; lineEnd != -1; lineStart = lineEnd + 1, lineEnd = str.indexOf(QLatin1Char('\n'), lineStart)) {
        mCurrentLine = str.mid(lineStart, lineEnd - lineStart);
        state = highlightLine(mCurrentLine, state);
        *mStream << QStringLiteral("<br>");
    }
    if (lineStart < str.size()) { // remaining content if str isn't ending with a newline
        mCurrentLine = str.mid(lineStart);
        state = highlightLine(mCurrentLine, state);
    }
    *mStream << QLatin1String("</code>");
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

    *mStream << mCurrentLine.mid(offset, length).toHtmlEscaped().replace(QLatin1Char(' '), QLatin1String("&nbsp;"));

    if (!format.isDefaultTextStyle(theme())) {
        *mStream << QStringLiteral("</span>");
    }
}
