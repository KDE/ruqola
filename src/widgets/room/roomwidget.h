/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "messages/message.h"
#include "room.h"
#include "roomheaderwidget.h"
#include <QPointer>
#include <QVBoxLayout>
#include <QWidget>

class RoomHeaderWidget;
class RocketChatAccount;
class UsersInRoomFlowWidget;
class RoomCounterInfoWidget;
class Room;
class RoomWidgetBase;
class ReconnectInfoWidget;
class OtrWidget;
class OffLineWidget;
class PluginTextMessageWidget;

namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoomWidget(QWidget *parent = nullptr);
    ~RoomWidget() override;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    [[nodiscard]] Room::RoomType roomType() const;

    Room *room() const;

    void slotShowListOfUsersInRoom(bool checked);
    void setChannelSelected(const QString &roomId, Room::RoomType roomType);
    void storeRoomSettings();

    void setLayoutSpacing(int spacing);
    void scrollToMessageId(const QString &messageId);

    void forceLineEditFocus();

Q_SIGNALS:
    void selectChannelRequested(const QString &channelId);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearNotification();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateRoomHeader();
    LIBRUQOLAWIDGETS_NO_EXPORT void connectRoom();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChangeFavorite(bool b);
    LIBRUQOLAWIDGETS_NO_EXPORT void setRoomType(Room::RoomType roomType);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotEncryptedChanged(bool b);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotGoBackToRoom();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreatePrivateDiscussion(const QString &userName);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotActionRequested(RoomHeaderWidget::ChannelActionType type);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowDiscussions();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowThreads();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowFileAttachments();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotStarredMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPinnedMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowMentions();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConfigureNotification();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadHistory();
    LIBRUQOLAWIDGETS_NO_EXPORT void
    slotOpenThreadRequested(const QString &threadMessageId, const QString &threadMessagePreview, bool threadIsFollowing, const Message &threadMessage);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConfigureAutoTranslate();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChannelInfoRequested();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotInviteUsers();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddUsersInRoom();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateListView();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVideoChat();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPruneMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateRoomCounterInfoWidget();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotExportMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void clearBeforeSwitching();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTeamChannelsRequested();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotJumpToUnreadMessage(qint64 numberOfMessage);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotGotoMessage(const QString &messageId, const QString &messageDateTimeUtc);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenTeamRequested(const QString &teamId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTryReconnect();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDisplayReconnectWidget(int seconds);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoginStatusChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCloseOtr();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRefreshOtrKeys();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCallRequested();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextToSpeech(const QString &messageText);
    LIBRUQOLAWIDGETS_NO_EXPORT void createOtrWidget();
    LIBRUQOLAWIDGETS_NO_EXPORT void createOffLineWidget();
    LIBRUQOLAWIDGETS_NO_EXPORT void createRoomReconnectInfoWidget();
    LIBRUQOLAWIDGETS_NO_EXPORT void createPluginTextMessateWidget();

    Room::RoomType mRoomType = Room::RoomType::Unknown;

    RoomWidgetBase *const mRoomWidgetBase;

    RoomHeaderWidget *const mRoomHeaderWidget;
    QPointer<Room> mRoom;
    UsersInRoomFlowWidget *const mUsersInRoomFlowWidget;
    RoomCounterInfoWidget *const mRoomCounterInfoWidget;
    ReconnectInfoWidget *mRoomReconnectInfoWidget = nullptr;
    PluginTextMessageWidget *mPluginTextMessageWidget = nullptr;
    OtrWidget *mOtrWidget = nullptr;
    OffLineWidget *mOffLineWidget = nullptr;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QVBoxLayout *mRoomWidgetLayout = nullptr;
};
