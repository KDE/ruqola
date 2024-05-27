/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
class MessagesModel;

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
    [[nodiscard]] QString text() const;

    MessageTextEdit *messageTextEdit() const;

    [[nodiscard]] EditingMode mode() const;
    void setMode(EditingMode mode);

    void setRoomId(const QByteArray &roomId);
    [[nodiscard]] QByteArray roomId() const;

    [[nodiscard]] QByteArray messageIdBeingEdited() const;
    void setMessageIdBeingEdited(const QByteArray &messageIdBeingEdited);

    void setEditMessage(const QByteArray &messageId, const QString &text);
    void setQuoteMessage(const QString &permalink, const QString &text);

    bool handleMimeData(const QMimeData *mimeData);

    void clearMessageIdBeingEdited();

    [[nodiscard]] QByteArray threadMessageId() const;
    void setThreadMessageId(const QByteArray &threadMessageId, const QString &text = {}, bool replyInDialogBox = false);

    void slotPublicSettingChanged();
    void slotOwnUserPreferencesChanged();

    [[nodiscard]] QString quotePermalink() const;

    [[nodiscard]] QString quoteText() const;

    void setRoomName(const QString &roomName, bool thread = false);
Q_SIGNALS:
    void keyPressed(QKeyEvent *ev);
    void threadMessageIdChanged(const QByteArray &threadMessageId, const QString &text);
    void quoteMessageChanged(const QString &permalink, const QString &text);
    void createUploadJob(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSendMessage(const QString &msg);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextEditing(bool clearNotification);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSendFile();
    LIBRUQOLAWIDGETS_NO_EXPORT void keyPressedInLineEdit(QKeyEvent *ev);
    LIBRUQOLAWIDGETS_NO_EXPORT void textEditClicked();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT MessagesModel *messageModel() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void clearEditingMode();
    LIBRUQOLAWIDGETS_NO_EXPORT void sendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSendVideoMessage();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSendSoundMessage();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPrivateSettingsChanged();

    QByteArray mThreadMessageId;
    QByteArray mMessageIdBeingEdited;
    QString mQuotePermalink;
    QString mQuoteText;
    EditingMode mMode = EditingMode::NewMessage;
    MessageTextEdit *const mMessageTextEdit;
    QToolButton *const mSendFileButton;
    QToolButton *const mVideoMessageButton;
    QToolButton *const mSoundMessageButton;
    QToolButton *const mEmoticonButton;
    QToolButton *const mSendMessageButton;
    EmoticonMenuWidget *mEmoticonMenuWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    bool mReplyInThreadDialogBox = false;
};
