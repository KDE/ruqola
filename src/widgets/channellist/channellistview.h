/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room.h"
#include <QTreeView>

#include "libruqolawidgets_private_export.h"

class RoomFilterProxyModel;
class RoomListHeadingsProxyModel;
class ChannelListDelegate;
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelListView : public QTreeView
{
    Q_OBJECT
public:
    struct ChannelSelectedInfo {
        QString roomId;
        QString roomName;
        Room::RoomType roomType;
        Utils::AvatarInfo avatarInfo;
    };

    explicit ChannelListView(QWidget *parent = nullptr);
    ~ChannelListView() override;

    [[nodiscard]] RoomFilterProxyModel *filterModel() const;

    void selectChannelRequested(const QString &channelId, const QString &messageId);
    void selectNextUnreadChannel();

    enum class Direction {
        Up,
        Down,
    };
    void selectNextChannel(Direction direction = Direction::Down, bool switchToNextUnreadChannel = false);

    [[nodiscard]] bool selectChannelByRoomNameRequested(const QString &selectedRoomName);

    void channelSelected(const QModelIndex &index);

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);
    [[nodiscard]] bool selectChannelByRoomIdRequested(const QString &identifier);

Q_SIGNALS:
    void roomSelected(const ChannelListView::ChannelSelectedInfo &roomInfo);
    void roomPressed(const QString &roomId);
    void selectMessageIdRequested(const QString &messageId);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClicked(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotHideChannel(const QModelIndex &index, Room::RoomType roomType);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLeaveChannel(const QModelIndex &index, Room::RoomType roomType);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChangeFavorite(const QModelIndex &index, bool isFavorite);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMarkAsChannel(const QModelIndex &index, bool markAsRead);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConvertToTeam(const QModelIndex &index, Room::RoomType roomType);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChannelConvertToTeamDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotGroupConvertToTeamDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMoveToTeam(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConvertToChannel(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConfigureNotification(Room *room);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool selectChannelByRoomIdOrRoomName(const QString &id, bool roomId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoomRemoved(const QString &roomId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPressed(const QModelIndex &index);

    ChannelListDelegate *const mChannelListDelegate;
    RoomListHeadingsProxyModel *const mRoomListHeadingsProxyModel;
    RoomFilterProxyModel *const mRoomFilterProxyModel;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QMetaObject::Connection mUpdateChannelViewConnect;
};
Q_DECLARE_TYPEINFO(ChannelListView::ChannelSelectedInfo, Q_RELOCATABLE_TYPE);
