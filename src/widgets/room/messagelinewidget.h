/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "dialogs/uploadfiledialog.h"
#include "libruqolawidgets_private_export.h"
#include "uploadfilejob.h"
#include <QPointer>
#include <QWidget>

class MessageTextEdit;
class QToolButton;
class EmoticonMenuWidget;
class RocketChatAccount;
class QMimeData;
class MessageModel;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageLineWidget : public QWidget
{
    Q_OBJECT
public:
    enum class EditingMode {
        EditMessage,
        NewMessage,
    };

    explicit MessageLineWidget(QWidget *parent = nullptr);
    ~MessageLineWidget() override;

    void setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog);

    void setText(const QString &text);
    Q_REQUIRED_RESULT QString text() const;

    MessageTextEdit *messageTextEdit() const;

    Q_REQUIRED_RESULT EditingMode mode() const;
    void setMode(EditingMode mode);

    void setRoomId(const QString &roomId);
    Q_REQUIRED_RESULT QString roomId() const;

    Q_REQUIRED_RESULT QString messageIdBeingEdited() const;
    void setMessageIdBeingEdited(const QString &messageIdBeingEdited);

    void setEditMessage(const QString &messageId, const QString &text);
    void setQuoteMessage(const QString &permalink, const QString &text);

    bool handleMimeData(const QMimeData *mimeData);

    void clearMessageIdBeingEdited();

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId, const QString &text = {}, bool replyInDialogBox = false);

    void slotPublicSettingChanged();
    void slotOwnUserPreferencesChanged();

    Q_REQUIRED_RESULT QString quotePermalink() const;

    Q_REQUIRED_RESULT QString quoteText() const;

Q_SIGNALS:
    void keyPressed(QKeyEvent *ev);
    void threadMessageIdChanged(const QString &threadMessageId, const QString &text);
    void quoteMessageChanged(const QString &permalink, const QString &text);
    void createUploadJob(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info);

private:
    void slotSendMessage(const QString &msg);
    void slotTextEditing(bool clearNotification);
    void slotSendFile();
    void keyPressedInLineEdit(QKeyEvent *ev);
    void textEditClicked();
    Q_REQUIRED_RESULT MessageModel *messageModel() const;
    void clearEditingMode();
    void sendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo);
    void slotSendVideoMessage();
    void slotSendSoundMessage();

    QString mRoomId;
    QString mThreadMessageId;
    QString mMessageIdBeingEdited;
    QString mQuotePermalink;
    QString mQuoteText;
    EditingMode mMode = EditingMode::NewMessage;
    MessageTextEdit *const mMessageTextEdit;
    QToolButton *const mSendFile;
    QToolButton *const mEmoticonButton;
    QToolButton *const mSendMessageButton;
    QToolButton *const mVideoMessageButton;
    QToolButton *const mSoundMessageButton;
    EmoticonMenuWidget *mEmoticonMenuWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    bool mReplyInThreadDialogBox = false;
};
