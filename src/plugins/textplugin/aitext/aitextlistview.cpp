/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextlistview.h"

#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QMenu>

AiTextListView::AiTextListView(QWidget *parent)
    : QListView(parent)
{
    setDragEnabled(false);
}

AiTextListView::~AiTextListView() = default;

void AiTextListView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    auto addAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18nc("@action", "Add…"), &menu);
    connect(addAction, &QAction::triggered, this, [this]() {
        // TODO
    });
    menu.addAction(addAction);
    const QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        auto editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18nc("@action", "Modify…"), &menu);
        connect(editAction, &QAction::triggered, this, [index, this]() {
            /*
            const QByteArray uuid = index.data(TextAutoGenerateChatModel::UuidRole).toByteArray();
            if (!uuid.isEmpty()) {
                edit(index);
            }
            */
        });
        menu.addAction(editAction);

        menu.addSeparator();
        auto removeAction = new QAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18nc("@action", "Remove…"), &menu);
        connect(removeAction, &QAction::triggered, this, [index]() {
            /*
            const QByteArray uuid = index.data(TextAutoGenerateChatModel::UuidRole).toByteArray();
            if (!uuid.isEmpty()) {
                TextAutogenerateManager::self()->textAutoGenerateChatModel()->removeDiscussion(uuid);
            }
            */
        });
        menu.addAction(removeAction);
        // TODO
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}
