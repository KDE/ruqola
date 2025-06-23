/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texthighlighter.h"

#include <KSyntaxHighlighting/Format>
#include <KSyntaxHighlighting/State>
#include <KSyntaxHighlighting/Theme>

#include <QTextStream>

using namespace Qt::Literals::StringLiterals;
TextHighlighter::TextHighlighter(QTextStream *stream)
    : mStream(stream)
{
}

void TextHighlighter::highlight(const QString &str)
{
    *mStream << u"<code>"_s;
    KSyntaxHighlighting::State state;

    int lineStart = 0;
    int lineEnd = str.indexOf(u'\n');

    for (; lineEnd != -1; lineStart = lineEnd + 1, lineEnd = str.indexOf(u'\n', lineStart)) {
        mCurrentLine = str.mid(lineStart, lineEnd - lineStart);
        state = highlightLine(mCurrentLine, state);
        *mStream << u"<br>"_s;
    }
    if (lineStart < str.size()) { // remaining content if str isn't ending with a newline
        mCurrentLine = str.mid(lineStart);
        state = highlightLine(mCurrentLine, state);
    }
    *mStream << "</code>"_L1;
}

void TextHighlighter::applyFormat(int offset, int length, const KSyntaxHighlighting::Format &format)
{
    if (!format.isDefaultTextStyle(theme())) {
        *mStream << u"<span style=\""_s;
        if (format.hasTextColor(theme())) {
            *mStream << u"color:"_s << format.textColor(theme()).name() << u";"_s;
        }
        if (format.hasBackgroundColor(theme())) {
            *mStream << u"background-color:"_s << format.backgroundColor(theme()).name() << u";"_s;
        }
        if (format.isBold(theme())) {
            *mStream << u"font-weight:bold;"_s;
        }
        if (format.isItalic(theme())) {
            *mStream << u"font-style:italic;"_s;
        }
        if (format.isUnderline(theme())) {
            *mStream << u"text-decoration:underline;"_s;
        }
        if (format.isStrikeThrough(theme())) {
            *mStream << u"text-decoration:line-through;"_s;
        }
        *mStream << u"\">"_s;
    }
    QString str = mCurrentLine.mid(offset, length).toHtmlEscaped();
    str = str.replace(u'\t', "    "_L1);
    str = str.replace(u' ', "&nbsp;"_L1);
    *mStream << str;

    if (!format.isDefaultTextStyle(theme())) {
        *mStream << u"</span>"_s;
    }
}
