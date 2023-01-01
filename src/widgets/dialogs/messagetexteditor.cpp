/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetexteditor.h"

MessageTextEditor::MessageTextEditor(QWidget *parent)
    : KTextEdit(parent)
{
    setAcceptRichText(false);
}

MessageTextEditor::~MessageTextEditor() = default;

QSize MessageTextEditor::sizeHint() const
{
    // The width of the QTextDocument is the current widget width, so this is somewhat circular logic.
    // But I don't really want to redo the layout with a different width like idealWidth(), seems slow.
    const QSize docSize = document()->size().toSize();
    const int margin = int(document()->documentMargin());
    return {docSize.width() + margin, qMin(300, 2 * docSize.height()) + margin};
}

QSize MessageTextEditor::minimumSizeHint() const
{
    const int margin = int(document()->documentMargin());
    return {300, fontMetrics().height() * 2 + margin};
}
