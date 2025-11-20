/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include "room.h"
#include <QPointer>
class QLabel;
class QToolButton;
class ChannelActionPopupMenu;
class RocketChatAccount;
class TeamNameLabel;
class RoomHeaderLabel;
class AvatarCacheManager;
class ActionButtonsGenerator;
class PluginToolInterface;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomHeaderWidget : public QWidget
{
    Q_OBJECT
public:
    enum ChannelActionType : uint8_t {
        ShowMentions = 1,
        ShowPinned = 2,
        ShowStarred = 3,
        ShowDiscussions = 4,
        ShowThreads = 5,
        ShowAttachment = 6,
        Notification = 7,
        AutoTranslate = 8,
        InviteUsers = 9,
        AddUsersInRoom = 10,
        VideoChat = 11,
        PruneMessages = 12,
        ExportMessages = 13,
        OtrMessages = 14,
        EncryptMessages = 15,
    };
    Q_ENUM(ChannelActionType)

    explicit RoomHeaderWidget(QWidget *parent = nullptr);
    ~RoomHeaderWidget() override;
    void setRoomName(const QString &name);
    [[nodiscard]] QString roomName() const;
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
    void setIsDirectGroup(bool state);
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
    void openTeam(const QByteArray &identifier);
    void uiInteractionRequested(const QJsonObject &obj);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDisabledEncryption();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotActionButtonChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOfflineModeChanged();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QByteArray roomId() const;
    QList<PluginToolInterface *> mPluginToolInterface;
    QLabel *const mRoomName;
    TeamNameLabel *const mTeamName;
    RoomHeaderLabel *const mRoomHeaderLabel;
    QToolButton *const mDiscussionBackButton;
    QToolButton *const mFavoriteButton;
    QLabel *const mRoomIcon;
    QToolButton *const mCallButton;
    QToolButton *const mChannelInfoButton;
    QToolButton *const mTeamChannelsButton;
    QToolButton *const mListOfUsersButton;
    QToolButton *const mSearchMessageButton;
    QToolButton *mPluginButton = nullptr;

    QToolButton *const mEncryptedButton;
    QToolButton *const mChannelActionButton;
    QToolButton *const mAIActionButton;
    ChannelActionPopupMenu *mChannelActionPopupMenu = nullptr;
    AvatarCacheManager *const mAvatarCacheManager;
    ActionButtonsGenerator *const mActionButtonsGenerator;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QPointer<Room> mRoom;
};
