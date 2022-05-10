/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room.h"
#include <QListView>

#include "libruqolawidgets_private_export.h"

class RoomFilterProxyModel;
class RoomListHeadingsProxyModel;
class ChannelListDelegate;
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelListView : public QListView
{
    Q_OBJECT
public:
    explicit ChannelListView(QWidget *parent = nullptr);
    ~ChannelListView() override;

    QAbstractItemModel *model() const;
    Q_REQUIRED_RESULT RoomFilterProxyModel *filterModel() const;
    void setFilterModel(RoomFilterProxyModel *model);

    void selectChannelRequested(const QString &channelId);
    void selectNextUnreadChannel();

    Q_REQUIRED_RESULT bool selectChannelByRoomNameRequested(const QString &selectedRoomName);

    void channelSelected(const QModelIndex &index);

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);
    Q_REQUIRED_RESULT bool selectChannelByRoomIdRequested(const QString &identifier);

Q_SIGNALS:
    void roomSelected(const QString &roomName, const QString &roomId, Room::RoomType roomType);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void setModel(QAbstractItemModel *model) override;
    void slotClicked(const QModelIndex &index);
    void slotHideChannel(const QModelIndex &index, Room::RoomType roomType);
    void slotLeaveChannel(const QModelIndex &index, Room::RoomType roomType);
    void slotChangeFavorite(const QModelIndex &index, bool isFavorite);
    void slotMarkAsChannel(const QModelIndex &index, bool markAsRead);
    void slotConvertToTeam(const QModelIndex &index, Room::RoomType roomType);
    void slotChannelConvertToTeamDone(const QJsonObject &obj);
    void slotGroupConvertToTeamDone(const QJsonObject &obj);
    void slotMoveToTeam(const QModelIndex &index);
    void slotConvertToChannel(const QModelIndex &index);
    void slotConfigureNotification(Room *room);
    Q_REQUIRED_RESULT bool selectChannelByRoomIdOrRoomName(const QString &id, bool roomId);

    ChannelListDelegate *const mChannelListDelegate;
    RoomListHeadingsProxyModel *const mRoomListHeadingsProxyModel;
};
