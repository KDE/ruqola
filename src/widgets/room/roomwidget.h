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

#ifndef ROOMWIDGET_H
#define ROOMWIDGET_H

#include <QPointer>
#include <QWidget>

#include "dialogs/uploadfiledialog.h"
#include "libruqolawidgets_private_export.h"
#include "roomheaderwidget.h"

class RoomHeaderWidget;
class MessageListView;
class MessageLineWidget;
class RoomWrapper;
class ReadOnlyLineEditWidget;
class QStackedWidget;
class QHBoxLayout;
class RocketChatAccount;
class UsersInRoomFlowWidget;

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

    RoomWrapper *roomWrapper() const;

    void slotShowListOfUsersInRoom(bool checked);
    void setChannelSelected(const QString &roomId, const QString &roomType);

Q_SIGNALS:
    void selectChannelRequested(const QString &channelId);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void slotClearNotification();
    void updateRoomHeader();
    void connectRoomWrapper();
    void slotChangeFavorite(bool b);
    void keyPressedInLineEdit(QKeyEvent *ev);
    void slotShowThreadMessage(const QString &threadMessageId);
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

    QString mRoomId;
    QString mRoomType;

    RoomHeaderWidget *mRoomHeaderWidget = nullptr;
    MessageListView *mMessageListView = nullptr;
    QWidget *mMessageThreadWidget = nullptr;
    QHBoxLayout *mMessageThreadLayout = nullptr;
    QLabel *mMessageThreadLabel = nullptr;
    QToolButton *mMessageThreadButton = nullptr;
    MessageLineWidget *mMessageLineWidget = nullptr;
    RoomWrapper *mRoomWrapper = nullptr;
    QStackedWidget *mStackedWidget = nullptr;
    ReadOnlyLineEditWidget *mReadOnlyLineEditWidget = nullptr;
    UsersInRoomFlowWidget *mUsersInRoomFlowWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};

#endif // ROOMWIDGET_H
