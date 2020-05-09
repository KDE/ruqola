/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "channellistview.h"

#include "accountmanager.h"
#include "channellistdelegate.h"
#include "model/accountschannelsmodel.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

ChannelListView::ChannelListView(QWidget *parent)
    : QTreeView(parent)
    , mChannelListDelegate(new ChannelListDelegate(this))
{
    setItemDelegate(mChannelListDelegate);
    setUniformRowHeights(true);
    setHeaderHidden(true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this, &ChannelListView::clicked, this, &ChannelListView::activateChannel);

    auto model = new AccountsChannelsModel(this);
    auto syncCurrentIndex = [this, model](const RocketChatAccount *acct){
        if (model->isFiltered())
            return; // We don't keep in sync when filtering

        const auto index = model->findRoomById(mCurrentChannelId, acct);
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    };

    connect(model, &AccountsChannelsModel::modelChanged, this, [syncCurrentIndex]{
        syncCurrentIndex(Ruqola::self()->rocketChatAccount());
    });
    connect(this, &ChannelListView::channelActivated, this, [syncCurrentIndex](const QString &acct){
        syncCurrentIndex(Ruqola::self()->accountManager()->rocketChatAccountModel()->account(acct));
    });
    QTreeView::setModel(model);
}


void ChannelListView::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mChannelListDelegate->setCurrentRocketChatAccount(currentRocketChatAccount);
}

ChannelListView::~ChannelListView()
{
}

void ChannelListView::setModel(QAbstractItemModel *)
{
    Q_ASSERT_X(false, "", "Cannot change the channel list view model");
}

void ChannelListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        return;
    }
    QMenu menu(this);

    const QString roomType = index.data(RoomModel::RoomType).toString();
    auto hideChannel = new QAction(QIcon::fromTheme(QStringLiteral("hide_table_row")), i18n("Hide Channel"), &menu);
    connect(hideChannel, &QAction::triggered, this, [=]() {
        slotHideChannel(index, roomType);
    });
    menu.addAction(hideChannel);

    const bool isUnRead = index.data(RoomModel::RoomAlert).toBool();
    const QString actionMarkAsText = isUnRead ? i18n("Mark As Read") : i18n("Mark As Unread");
    auto markAsChannel = new QAction(actionMarkAsText, &menu);
    connect(markAsChannel, &QAction::triggered, this, [=]() {
        slotMarkAsChannel(index, isUnRead);
    });
    menu.addAction(markAsChannel);

    const bool isFavorite = index.data(RoomModel::RoomFavorite).toBool();
    const QString actionFavoriteText = isFavorite ? i18n("Unset as Favorite") : i18n("Set as Favorite");
    auto favoriteAction = new QAction(QIcon::fromTheme(QStringLiteral("favorite")), actionFavoriteText, &menu);
    connect(favoriteAction, &QAction::triggered, this, [=]() {
        slotChangeFavorite(index, isFavorite);
    });
    menu.addAction(favoriteAction);

    if (roomType == QLatin1String("c") || roomType == QLatin1String("p")) { // Not direct channel
        auto separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
        auto quitChannel = new QAction(QIcon::fromTheme(QStringLiteral("dialog-close")), i18n("Quit Channel"), &menu);
        connect(quitChannel, &QAction::triggered, this, [=]() {
            slotLeaveChannel(index, roomType);
        });
        menu.addAction(quitChannel);
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}

void ChannelListView::slotMarkAsChannel(const QModelIndex &index, bool markAsRead)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString roomId = index.data(RoomModel::RoomId).toString();
    if (markAsRead) {
        rcAccount->markRoomAsRead(roomId);
    } else {
        rcAccount->markRoomAsUnRead(roomId);
    }
}

AccountsChannelsModel *ChannelListView::rooms() const
{
    Q_ASSERT(qobject_cast<AccountsChannelsModel*>(QTreeView::model()));
    return static_cast<AccountsChannelsModel*>(QTreeView::model());
}

void ChannelListView::activateChannel(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    if (!index.parent().isValid())
    {
        // This is an account index, activate last selected room
        if (auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount*>())
            activateChannelById(account->settings()->lastSelectedRoom());
        return;
    }

    const QString roomId = index.data(RoomModel::RoomId).toString();
    const QString roomType = index.data(RoomModel::RoomType).toString();
    const QString acct = rooms()->accountForIndex(index);
    mCurrentChannelId = roomId;
    Q_EMIT channelActivated(acct, roomId, roomType);
}

void ChannelListView::slotHideChannel(const QModelIndex &index, const QString &roomType)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString roomId = index.data(RoomModel::RoomId).toString();
    rcAccount->hideRoom(roomId, roomType);
}

void ChannelListView::slotLeaveChannel(const QModelIndex &index, const QString &roomType)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString roomId = index.data(RoomModel::RoomId).toString();
    rcAccount->leaveRoom(roomId, roomType);
}

void ChannelListView::slotChangeFavorite(const QModelIndex &index, bool isFavorite)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString roomId = index.data(RoomModel::RoomId).toString();
    rcAccount->changeFavorite(roomId, !isFavorite);
}

void ChannelListView::activateChannelById(const QString &channelId)
{
    activateChannel(rooms()->findRoomById(channelId, Ruqola::self()->rocketChatAccount()));
}

bool ChannelListView::activateChannelByRoomName(const QString &selectedRoomName)
{
    const auto roomIdx = rooms()->findRoomByName(selectedRoomName, Ruqola::self()->rocketChatAccount());
    if (!roomIdx.isValid())
        return false;

    activateChannel(roomIdx);
    return true;
}

void ChannelListView::moveSelectionDown()
{
    const auto index = [&]() -> QModelIndex {
        const auto current = currentIndex().isValid() ? currentIndex() : rooms()->index(0, 0);
        if (!current.isValid())
            return {};

        const auto useParent = current.parent();
        if (useParent.isValid())
        {
            const auto room = current.sibling(current.row() + 1, 0);
            if (room.isValid())
                return room;
        }

        auto account = useParent.isValid() ? useParent.sibling(useParent.row() + 1, 0) : current;
        while (account.isValid())
        {
            if (rooms()->hasChildren(account))
                return rooms()->index(0, 0, account);
            account = account.sibling(account.row() + 1, account.column());
        }
        return {};
    }();

    if (index.isValid())
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void ChannelListView::moveSelectionUp()
{
    const auto index = [&]() -> QModelIndex {
        const auto current = [&]{
            if (currentIndex().isValid())
                return currentIndex();

            const auto lastParent = rooms()->index(rooms()->rowCount() - 1, 0);
            if (!lastParent.isValid())
                return lastParent;

            const auto lastChild = rooms()->index(rooms()->rowCount(lastParent) - 1, 0, lastParent);
            return lastChild.isValid() ? lastChild : lastParent;
        }();
        if (!current.isValid())
            return {};

        const auto useParent = current.parent();
        if (useParent.isValid())
        {
            const auto room = current.sibling(current.row() - 1, 0);
            if (room.isValid())
                return room;
        }

        auto account = useParent.isValid() ? useParent.sibling(useParent.row() - 1, 0) : current;
        while (account.isValid())
        {
            if (rooms()->hasChildren(account))
                return rooms()->index(rooms()->rowCount(account) - 1, 0, account);
            account = account.sibling(account.row() - 1, account.column());
        }

        return {};
    }();

    if (index.isValid())
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void ChannelListView::setFilterString(const QString &filter)
{
    rooms()->setFilterString(filter);
}
