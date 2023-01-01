/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
        OtrMessages = 15,
        EncryptMessages = 16,
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
    void setCallEnabled(bool b);
Q_SIGNALS:
    void favoriteChanged(bool b);
    void encryptedChanged(bool b);
    void goBackToRoom();
    void listOfUsersChanged(bool b);
    void searchMessageRequested();
    void actionRequested(RoomHeaderWidget::ChannelActionType type);
    void channelInfoRequested();
    void teamChannelsRequested();
    void callRequested();
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
    QToolButton *const mCallButton;
    ChannelActionPopupMenu *mChannelActionPopupMenu = nullptr;
};
