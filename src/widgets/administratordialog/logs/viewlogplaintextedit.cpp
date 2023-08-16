/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "viewlogplaintextedit.h"
#include <QMenu>

ViewLogPlainTextEdit::ViewLogPlainTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
}

ViewLogPlainTextEdit::~ViewLogPlainTextEdit() = default;

void ViewLogPlainTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *popup = createStandardContextMenu();
    if (popup) {
        popup->exec(event->globalPos());
        delete popup;
    }
}
