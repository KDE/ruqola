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

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include "model/roommodel.h"
class QLabel;
class QToolButton;
class ChannelActionPopupMenu;
class RocketChatAccount;
class Room;
class TeamNameLabel;
class RoomHeaderLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomHeaderWidget : public QWidget
{
    Q_OBJECT
public:
    enum ChannelActionType {
        ShowMentions = 1,
        ShowPinned = 2,
        ShowStarred = 3,
        ShowSnippered = 4,
        ShowDiscussions = 5,
        ShowThreads = 6,
        ShowAttachment = 7,
        Notification = 8,
        AutoTranslate = 9,
        InviteUsers = 10,
        AddUsersInRoom = 11,
        VideoChat = 12,
        PruneMessages = 13,
        ExportMessages = 14,
        FollowMessages = 15,
    };
    Q_ENUM(ChannelActionType)

    explicit RoomHeaderWidget(QWidget *parent = nullptr);
    ~RoomHeaderWidget() override;
    void setRoomName(const QString &name);
    Q_REQUIRED_RESULT QString roomName() const;
    void setRoomAnnouncement(const QString &name);
    void setRoomTopic(const QString &name);

    void setFavoriteStatus(bool b);
    void setEncypted(bool b);

    void setIsDiscussion(bool isDiscussion);

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    void setRoom(Room *room);

    void setIsMainTeam(bool isMainTeam);
    void setTeamRoomInfo(const Room::TeamRoomInfo &name);
Q_SIGNALS:
    void favoriteChanged(bool b);
    void encryptedChanged(bool b);
    void goBackToRoom();
    void listOfUsersChanged(bool b);
    void searchMessageRequested();
    void actionRequested(RoomHeaderWidget::ChannelActionType type);
    void channelInfoRequested();
    void teamChannelsRequested();
    void openTeam(const QString &identifier);

private:
    QLabel *const mRoomName;
    TeamNameLabel *const mTeamName;
    RoomHeaderLabel *const mRoomHeaderLabel;
    QToolButton *const mFavoriteButton;
    QToolButton *const mEncryptedButton;
    QToolButton *const mDiscussionBackButton;
    QToolButton *const mListOfUsersButton;
    QToolButton *const mSearchMessageButton;
    QToolButton *const mChannelAction;
    QToolButton *const mChannelInfoButton;
    QToolButton *const mTeamChannelsButton;
    ChannelActionPopupMenu *mChannelActionPopupMenu = nullptr;
};

