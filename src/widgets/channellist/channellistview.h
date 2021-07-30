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

#pragma once
#include "room.h"
#include <QListView>

#include "libruqolawidgets_private_export.h"

class RoomFilterProxyModel;
class ChannelListDelegate;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelListView : public QListView
{
    Q_OBJECT
public:
    explicit ChannelListView(QWidget *parent = nullptr);
    ~ChannelListView() override;

    QAbstractItemModel *model() const;
    RoomFilterProxyModel *filterModel() const;
    void setModel(QAbstractItemModel *model) override;

    void selectChannelRequested(const QString &channelId);
    void selectNextUnreadChannel();

    Q_REQUIRED_RESULT bool selectChannelByRoomNameRequested(const QString &selectedRoomName);

    void channelSelected(const QModelIndex &index);

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);
    Q_REQUIRED_RESULT bool selectChannelByRoomIdRequested(const QString &identifier);
Q_SIGNALS:
    void roomSelected(const QString &roomId, Room::RoomType roomType);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void slotClicked(const QModelIndex &index);
    void slotHideChannel(const QModelIndex &index, Room::RoomType roomType);
    void slotLeaveChannel(const QModelIndex &index, Room::RoomType roomType);
    void slotChangeFavorite(const QModelIndex &index, bool isFavorite);
    void slotMarkAsChannel(const QModelIndex &index, bool markAsRead);
    void slotConvertToTeam(const QModelIndex &index, Room::RoomType roomType);
    void slotChannelConvertToTeamDone(const QJsonObject &obj);
    void slotGroupConvertToTeamDone(const QJsonObject &obj);
    void slotMoveToTeam(const QModelIndex &index);

    ChannelListDelegate *const mChannelListDelegate;
    Q_REQUIRED_RESULT bool selectChannelByRoomIdOrRoomName(const QString &id, bool roomId);
};

