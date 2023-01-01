/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "messagelistview.h"
#include "uploadfilejob.h"
#include <QPointer>
#include <QWidget>

class RoomReplyThreadWidget;
class RoomQuoteMessageWidget;
class QStackedWidget;
class MessageLineWidget;
class ReadOnlyLineEditWidget;
class RocketChatAccount;
class Room;
class UploadFileProgressStatusListWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit RoomWidgetBase(MessageListView::Mode mode, QWidget *parent = nullptr);
    ~RoomWidgetBase() override;

    Q_REQUIRED_RESULT MessageListView *messageListView() const;

    Q_REQUIRED_RESULT MessageLineWidget *messageLineWidget() const;

    void setCurrentRocketChatAccount(RocketChatAccount *account);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    void updateListView();
    void updateRoomReadOnly(Room *room);
    void slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage, const QString &channelName);
    void closeQuoteAndThreadInfo();
Q_SIGNALS:
    void loadHistory();
    void createNewDiscussion(const QString &messageId, const QString &originalMessage);
    void textToSpeech(const QString &messageText);

private:
    void slotClearNotification();
    void slotShowQuoteMessage(const QString &permalink, const QString &text);
    void slotCreatePrivateDiscussion(const QString &userName);
    void keyPressedInLineEdit(QKeyEvent *ev);
    void slotShowThreadMessage(const QString &threadMessageId, const QString &text);
    void slotUploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info, int jobIdentifier, const QString &accountName);
    void slotSendFile(const RocketChatRestApi::UploadFileJob::UploadFileInfo &uploadFileInfo);
    void slotCancelUpload(int identifier);
    QString mRoomId;
    UploadFileProgressStatusListWidget *const mUploadFileProgressStatusListWidget;
    MessageListView *const mMessageListView;
    RoomReplyThreadWidget *const mRoomReplyThreadWidget;
    RoomQuoteMessageWidget *const mRoomQuoteMessageWidget;
    QStackedWidget *const mStackedWidget;
    MessageLineWidget *const mMessageLineWidget;
    ReadOnlyLineEditWidget *const mReadOnlyLineEditWidget;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
