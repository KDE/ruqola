/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "viewlogplaintextedit.h"
#include <KLocalizedString>
#include <QAction>
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
        auto action = new QAction(QIcon::fromTheme(QStringLiteral("document-save")), i18n("Save as &File"), this);
        connect(action, &QAction::triggered, this, &ViewLogPlainTextEdit::slotSaveAsFile);
        popup->addSeparator();
        popup->addAction(action);

        popup->exec(event->globalPos());
        delete popup;
    }
}

void ViewLogPlainTextEdit::slotSaveAsFile()
{
    // TODO
}
