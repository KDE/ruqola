/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextlistview.h"

#include "aitextlistviewdelegate.h"
#include "aitextmodel.h"

#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QMenu>

AiTextListView::AiTextListView(QWidget *parent)
    : QListView(parent)
    , mModel(new AiTextModel(this))
{
    setDragEnabled(false);
    setModel(mModel);
    setItemDelegate(new AiTextListViewDelegate(this));
}

AiTextListView::~AiTextListView() = default;

void AiTextListView::setAiTextInfos(const QList<AiTextInfo> &infos)
{
    mModel->setInfos(infos);
}

QList<AiTextInfo> AiTextListView::aiTextInfos() const
{
    return mModel->infos();
}

void AiTextListView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    auto addAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18nc("@action", "Add…"), &menu);
    connect(addAction, &QAction::triggered, this, [this]() {
        AiTextInfo info;
        info.setRequestText(QStringLiteral("foo")); // TODO fix me.
        mModel->addItem(std::move(info));
    });
    menu.addAction(addAction);
    const QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        auto editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18nc("@action", "Modify…"), &menu);
        connect(editAction, &QAction::triggered, this, [index, this]() {
            edit(index);
        });
        menu.addAction(editAction);

        menu.addSeparator();
        auto removeAction = new QAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18nc("@action", "Remove…"), &menu);
        connect(removeAction, &QAction::triggered, this, [index]() {
            // TODO remove it
            /*
            const QByteArray uuid = index.data(TextAutoGenerateChatModel::UuidRole).toByteArray();
            if (!uuid.isEmpty()) {
                TextAutogenerateManager::self()->textAutoGenerateChatModel()->removeDiscussion(uuid);
            }
            */
        });
        menu.addAction(removeAction);
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}

#include "moc_aitextlistview.cpp"
