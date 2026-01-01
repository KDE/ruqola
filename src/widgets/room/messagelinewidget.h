/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "commands/previewscommandjob.h"
#include "commands/runcommandjob.h"
#include "dialogs/uploadfiledialog.h"
#include "libruqolawidgets_private_export.h"
#include "uploadfilejob.h"
#include <QPointer>

class MessageTextEdit;
class QToolButton;
class EmoticonMenuWidget;
class RocketChatAccount;
class QMimeData;
class MessagesModel;
class PluginToolInterface;
class MessageLineExtraToolButton;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageLineWidget : public QWidget
{
    Q_OBJECT
public:
    enum class EditingMode : uint8_t {
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

    void setSendPreviewCommandInfo(const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo &info);
Q_SIGNALS:
    void keyPressed(QKeyEvent *ev);
    void threadMessageIdChanged(const QByteArray &threadMessageId, const QString &text);
    void quoteMessageChanged(const QString &permalink, const QString &text);
    void createUploadJob(const RocketChatRestApi::UploadFileJob::UploadFileInfo &info);
    void showCommandPreview(const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo &info);
    void uiInteractionRequested(const QJsonObject &obj);
    void addWebDavServer();

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
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool runCommand(const QString &msg, const QByteArray &roomId, const QByteArray &tmid);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRunCommandFailed(const RocketChatRestApi::RunCommandJob::RunCommandInfo &info);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool hasCommandPreview(const QString &msg, const QByteArray &roomId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOfflineModeChanged();

    QList<PluginToolInterface *> mPluginToolInterface;
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
    MessageLineExtraToolButton *const mMessageLineExtraToolButton;
    EmoticonMenuWidget *mEmoticonMenuWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    bool mReplyInThreadDialogBox = false;
};
