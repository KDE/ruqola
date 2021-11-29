/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegateutils.h"

#include <QApplication>
#include <QTextFrame>
#include <QTextFrameFormat>

std::unique_ptr<QTextDocument> MessageDelegateUtils::createTextDocument(bool useItalic, const QString &text, int width)
{
    std::unique_ptr<QTextDocument> doc(new QTextDocument);
    doc->setHtml(text);
    doc->setTextWidth(width);
    QFont font = qApp->font();
    font.setItalic(useItalic);
    doc->setDefaultFont(font);
    QTextFrame *frame = doc->frameAt(0);
    QTextFrameFormat frameFormat = frame->frameFormat();
    frameFormat.setMargin(0);
    frame->setFrameFormat(frameFormat);
    return doc;
}
