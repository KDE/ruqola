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

#include "uploadfilejob.h"
#include <QPointer>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include "roomheaderwidget.h"

class RoomHeaderWidget;
class RocketChatAccount;
class UsersInRoomFlowWidget;
class RoomCounterInfoWidget;
class Room;
class RoomWidgetBase;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoomWidget(QWidget *parent = nullptr);
    ~RoomWidget() override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    Q_REQUIRED_RESULT QString roomType() const;

    Room *room() const;

    void slotShowListOfUsersInRoom(bool checked);
    void setChannelSelected(const QString &roomId, const QString &roomType);
    void storeRoomSettings();

Q_SIGNALS:
    void selectChannelRequested(const QString &channelId);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void slotClearNotification();
    void updateRoomHeader();
    void connectRoom();
    void slotChangeFavorite(bool b);
    void setRoomType(const QString &roomType);
    void slotEncryptedChanged(bool b);
    void slotGoBackToRoom();
    void slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage);
    void slotCreatePrivateDiscussion(const QString &userName);
    void slotSearchMessages();
    void slotActionRequested(RoomHeaderWidget::ChannelActionType type);
    void slotShowDiscussions();
    void slotShowThreads();
    void slotShowFileAttachments();
    void slotStarredMessages();
    void slotPinnedMessages();
    void slotShowMentions();
    void slotSnipperedMessages();
    void slotConfigureNotification();
    void slotLoadHistory();
    void slotOpenThreadRequested(const QString &threadMessageId, const QString &threadMessagePreview);
    void slotConfigureAutoTranslate();
    void slotChannelInfoRequested();
    void slotInviteUsers();
    void slotAddUsersInRoom();
    void updateListView();
    void slotVideoChat();
    void slotPruneMessages();
    void slotUpdateRoomCounterInfoWidget();
    void slotExportMessages();
    void slotFollowMessages();

    QString mRoomType;

    RoomWidgetBase *const mRoomWidgetBase;

    RoomHeaderWidget *const mRoomHeaderWidget;
    Room *mRoom = nullptr;
    UsersInRoomFlowWidget *const mUsersInRoomFlowWidget;
    RoomCounterInfoWidget *const mRoomCounterInfoWidget;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};

