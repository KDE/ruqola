/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showbanneduserslistview.h"
#include "model/bannedusersmodel.h"
#include <KLocalizedString>
#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
using namespace Qt::Literals::StringLiterals;
ShowBannedUsersListView::ShowBannedUsersListView(QWidget *parent)
    : QListView(parent)
{
}

ShowBannedUsersListView::~ShowBannedUsersListView() = default;

void ShowBannedUsersListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        return;
    }
    QMenu menu(this);
    auto unbanUserAction = new QAction(i18nc("@action", "Unban User"), &menu);
    connect(unbanUserAction, &QAction::triggered, this, [this, index]() {
        slotUnbanUser(index);
    });
    menu.addAction(unbanUserAction);
    menu.exec(event->globalPos());
}

void ShowBannedUsersListView::slotUnbanUser(const QModelIndex &index)
{
    const QString userName = index.data(BannedUsersModel::UserName).toString();
    Q_EMIT unbanUser(userName);
}

#include "moc_showbanneduserslistview.cpp"
