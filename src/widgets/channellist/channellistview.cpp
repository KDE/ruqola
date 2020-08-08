/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "rocketchataccount.h"
#include "channellistdelegate.h"
#include "model/roomfilterproxymodel.h"

#include <KLocalizedString>

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

ChannelListView::ChannelListView(QWidget *parent)
    : QListView(parent)
{
    auto *delegate = new ChannelListDelegate(this);
    setItemDelegate(delegate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this, &ChannelListView::clicked, this, &ChannelListView::slotClicked);
}

ChannelListView::~ChannelListView()
{
}

RoomFilterProxyModel *ChannelListView::model() const
{
    return qobject_cast<RoomFilterProxyModel *>(QListView::model());
}

void ChannelListView::setModel(QAbstractItemModel *model)
{
    if (!qobject_cast<RoomFilterProxyModel *>(model)) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Need to pass a RoomFilterProxyModel instance!";
        return;
    }

    QListView::setModel(model);
}

void ChannelListView::slotClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        channelSelected(index);
    }
}

void ChannelListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        return;
    }
    QMenu menu(this);

    const QString roomType = index.data(RoomModel::RoomType).toString();
    QAction *hideChannel = new QAction(QIcon::fromTheme(QStringLiteral("hide_table_row")), i18n("Hide Channel"), &menu);
    connect(hideChannel, &QAction::triggered, this, [=]() {
        slotHideChannel(index, roomType);
    });
    menu.addAction(hideChannel);

    const bool isUnRead = index.data(RoomModel::RoomAlert).toBool();
    const QString actionMarkAsText = isUnRead ? i18n("Mark As Read") : i18n("Mark As Unread");
    QAction *markAsChannel = new QAction(actionMarkAsText, &menu);
    connect(markAsChannel, &QAction::triggered, this, [=]() {
        slotMarkAsChannel(index, isUnRead);
    });
    menu.addAction(markAsChannel);

    const bool isFavorite = index.data(RoomModel::RoomFavorite).toBool();
    const QString actionFavoriteText = isFavorite ? i18n("Unset as Favorite") : i18n("Set as Favorite");
    QAction *favoriteAction = new QAction(QIcon::fromTheme(QStringLiteral("favorite")), actionFavoriteText, &menu);
    connect(favoriteAction, &QAction::triggered, this, [=]() {
        slotChangeFavorite(index, isFavorite);
    });
    menu.addAction(favoriteAction);

    if (roomType == QLatin1String("c") || roomType == QLatin1String("p")) { //Not direct channel
        QAction *muteChannel = new QAction(QIcon::fromTheme(QStringLiteral("notifications-disabled")), i18n("Mute Channel"), &menu);
        muteChannel->setCheckable(true);
        muteChannel->setChecked(index.data(RoomModel::RoomMuted).toBool());
        connect(muteChannel, &QAction::toggled, this, [=](bool checked) {
            slotSetChannelMute(index, checked);
        });
        menu.addAction(muteChannel);

        auto *separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
        QAction *quitChannel = new QAction(QIcon::fromTheme(QStringLiteral("dialog-close")), i18n("Quit Channel"), &menu);
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
    const QString roomId = index.data(RoomModel::RoomID).toString();
    if (markAsRead) {
        rcAccount->markRoomAsRead(roomId);
    } else {
        rcAccount->markRoomAsUnRead(roomId);
    }
}

void ChannelListView::channelSelected(const QModelIndex &index)
{
    const QString roomId = index.data(RoomModel::RoomID).toString();
    const QString roomType = index.data(RoomModel::RoomType).toString();
    Q_EMIT roomSelected(roomId, roomType);
}

void ChannelListView::slotSetChannelMute(const QModelIndex &index, bool muted)
{
    const QString roomId = index.data(RoomModel::RoomID).toString();
    RoomModel *roomModel = static_cast<RoomModel*>(model()->sourceModel());
    Q_ASSERT(roomModel);

    roomModel->setRoomMute(roomId, muted);
}

void ChannelListView::slotHideChannel(const QModelIndex &index, const QString &roomType)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString roomId = index.data(RoomModel::RoomID).toString();
    rcAccount->hideRoom(roomId, roomType);
}

void ChannelListView::slotLeaveChannel(const QModelIndex &index, const QString &roomType)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString roomId = index.data(RoomModel::RoomID).toString();
    rcAccount->leaveRoom(roomId, roomType);
}

void ChannelListView::slotChangeFavorite(const QModelIndex &index, bool isFavorite)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString roomId = index.data(RoomModel::RoomID).toString();
    rcAccount->changeFavorite(roomId, !isFavorite);
}

void ChannelListView::selectChannelRequested(const QString &channelId)
{
    if (channelId.isEmpty()) {
        return;
    }
    RoomFilterProxyModel *filterModel = model();
    Q_ASSERT(filterModel);
    const int nRooms = filterModel->rowCount();
    if (nRooms == 0) {
        return; // too early, next chance when accountInitialized is emitted
    }
    for (int roomIdx = 0; roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = filterModel->index(roomIdx, 0);
        const auto roomId = roomModelIndex.data(RoomModel::RoomID).toString();
        if (roomId == channelId) {
            channelSelected(roomModelIndex);
            selectionModel()->setCurrentIndex(filterModel->index(roomIdx, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            return;
        }
    }
    qCWarning(RUQOLAWIDGETS_LOG) << "Room not found:" << channelId;
}

bool ChannelListView::selectChannelByRoomNameRequested(const QString &selectedRoomName)
{
    if (selectedRoomName.isEmpty()) {
        return false;
    }
    RoomFilterProxyModel *filterModel = model();
    for (int roomIdx = 0, nRooms = filterModel->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = filterModel->index(roomIdx, 0);
        const auto roomName = roomModelIndex.data(RoomModel::RoomName).toString();
        if (roomName == selectedRoomName) {
            channelSelected(roomModelIndex);
            selectionModel()->setCurrentIndex(filterModel->index(roomIdx, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            return true;
        }
    }
    return false;
}
