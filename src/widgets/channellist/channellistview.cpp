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
#include "channellistdelegate.h"
#include "model/roomfilterproxymodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "teams/channelsconverttoteamjob.h"
#include "teams/groupsconverttoteamjob.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

ChannelListView::ChannelListView(QWidget *parent)
    : QListView(parent)
    , mChannelListDelegate(new ChannelListDelegate(this))
{
    mChannelListDelegate->setObjectName(QStringLiteral("mChannelListDelegate"));
    setItemDelegate(mChannelListDelegate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this, &ChannelListView::clicked, this, &ChannelListView::slotClicked);
}

ChannelListView::~ChannelListView()
{
}

void ChannelListView::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mChannelListDelegate->setCurrentRocketChatAccount(currentRocketChatAccount);
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

    const auto roomType = index.data(RoomModel::RoomType).value<Room::RoomType>();
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

    if (roomType == Room::RoomType::Channel || roomType == Room::RoomType::Private) { // Not direct channel
        const bool mainTeam = index.data(RoomModel::RoomTeamIsMain).toBool();
        if (!mainTeam) {
            menu.addSeparator();
            auto convertToTeam = new QAction(i18n("Convert to Team"), &menu);
            connect(convertToTeam, &QAction::triggered, this, [=]() {
                slotConvertToTeam(index, roomType);
            });
            menu.addAction(convertToTeam);
        }

        menu.addSeparator();
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

void ChannelListView::slotConvertToTeam(const QModelIndex &index, Room::RoomType roomType)
{
    if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Are you sure to convert it to team? It can not be undo."), i18n("Convert to Team"))) {
        const QString roomId = index.data(RoomModel::RoomId).toString();
        switch (roomType) {
        case Room::RoomType::Unknown:
            qCWarning(RUQOLAWIDGETS_LOG) << "Unknow type used it's a bug";
            break;
        case Room::RoomType::Direct:
            qCWarning(RUQOLAWIDGETS_LOG) << "We can't convert Direct to Team. It's a bug";
            break;
        case Room::RoomType::Channel: {
            auto *rcAccount = Ruqola::self()->rocketChatAccount();
            auto job = new RocketChatRestApi::ChannelsConvertToTeamJob(this);
            job->setChannelId(roomId);
            rcAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::ChannelsConvertToTeamJob::channelConvertToTeamDone, this, &ChannelListView::slotChannelConvertToTeamDone);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelsConvertToTeamJob job";
            }
            break;
        }
        case Room::RoomType::Private: {
            auto *rcAccount = Ruqola::self()->rocketChatAccount();
            auto job = new RocketChatRestApi::GroupsConvertToTeamJob(this);
            job->setRoomId(roomId);
            rcAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::GroupsConvertToTeamJob::groupConvertToTeamDone, this, &ChannelListView::slotGroupConvertToTeamDone);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelsConvertToTeamJob job";
            }
            break;
        }
        }
    }
}

void ChannelListView::slotChannelConvertToTeamDone(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    // qDebug() << " obj "<< obj;
    // TODO
}

void ChannelListView::slotGroupConvertToTeamDone(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    // qDebug() << " obj "<< obj;
    // TODO
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

void ChannelListView::channelSelected(const QModelIndex &index)
{
    const QString roomId = index.data(RoomModel::RoomId).toString();
    const auto roomType = index.data(RoomModel::RoomType).value<Room::RoomType>();
    Q_EMIT roomSelected(roomId, roomType);
}

void ChannelListView::slotHideChannel(const QModelIndex &index, Room::RoomType roomType)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString roomId = index.data(RoomModel::RoomId).toString();
    rcAccount->hideRoom(roomId, roomType);
}

void ChannelListView::slotLeaveChannel(const QModelIndex &index, Room::RoomType roomType)
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
        const auto roomId = roomModelIndex.data(RoomModel::RoomId).toString();
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

void ChannelListView::selectNextUnreadChannel()
{
    RoomFilterProxyModel *filterModel = model();
    Q_ASSERT(filterModel);
    const int nRooms = filterModel->rowCount();
    if (nRooms == 0) {
        return; 
    }
    int roomIdx = 0; 
    // if we have a selection, start searching for the next unread channel there, otherwise start at the top
    const auto currentlySelectedIndex = selectionModel()->currentIndex();
    if (currentlySelectedIndex.isValid())
        roomIdx = currentlySelectedIndex.row();

    const int startIndex = roomIdx;
    // iterate through to the end, and then wrap around to the starting point
    while (roomIdx < nRooms || (roomIdx % nRooms < startIndex)) {
        const auto roomModelIndex = filterModel->index(roomIdx % nRooms, 0);
        const bool isUnRead = roomModelIndex.data(RoomModel::RoomAlert).toBool();
        if (isUnRead && roomModelIndex != currentlySelectedIndex) {
            channelSelected(roomModelIndex);
            selectionModel()->setCurrentIndex(filterModel->index(roomIdx % nRooms, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            return;
        }
        roomIdx++;
    }
}
