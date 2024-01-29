/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
class QVBoxLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit RoomWidgetBase(MessageListView::Mode mode, QWidget *parent = nullptr);
    ~RoomWidgetBase() override;

    [[nodiscard]] MessageListView *messageListView() const;

    [[nodiscard]] MessageLineWidget *messageLineWidget() const;

    void setCurrentRocketChatAccount(RocketChatAccount *account);

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    void updateListView();
    void updateRoomReadOnly(Room *room);
    void slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage, const QString &channelName);
    void closeQuoteAndThreadInfo();
Q_SIGNALS:
    void loadHistory();
    void createNewDiscussion(const QString &messageId, const QString &originalMessage);
    void textToSpeech(const QString &messageText);
    void errorMessage(const QString &message);
    void successMessage(const QString &message);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearNotification();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowQuoteMessage(const QString &permalink, const QString &text);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreatePrivateDiscussion(const QString &userName);
    LIBRUQOLAWIDGETS_NO_EXPORT void keyPressedInLineEdit(QKeyEvent *ev);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowThreadMessage(const QString &threadMessageId, const QString &text);
    LIBRUQOLAWIDGETS_NO_EXPORT void
    slotUploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info, int jobIdentifier, const QString &accountName);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSendFile(const RocketChatRestApi::UploadFileJob::UploadFileInfo &uploadFileInfo);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCancelUpload(int identifier);
    QString mRoomId;
    UploadFileProgressStatusListWidget *const mUploadFileProgressStatusListWidget;
    MessageListView *const mMessageListView;
    RoomReplyThreadWidget *const mRoomReplyThreadWidget;
    RoomQuoteMessageWidget *const mRoomQuoteMessageWidget;
    QStackedWidget *const mStackedWidget;
    MessageLineWidget *const mMessageLineWidget;
    ReadOnlyLineEditWidget *const mReadOnlyLineEditWidget;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QVBoxLayout *const mMainLayout;
};
