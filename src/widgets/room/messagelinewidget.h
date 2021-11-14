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

#include "dialogs/uploadfiledialog.h"
#include "libruqolawidgets_private_export.h"
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
    void setThreadMessageId(const QString &threadMessageId, bool replyInDialogBox = false);

    void slotPublicSettingChanged();
    void slotOwnUserPreferencesChanged();

    Q_REQUIRED_RESULT QString quotePermalink() const;

    Q_REQUIRED_RESULT QString quoteText() const;

Q_SIGNALS:
    void keyPressed(QKeyEvent *ev);
    void threadMessageIdChanged(const QString &threadMessageId);
    void quoteMessageChanged(const QString &permalink, const QString &text);

private:
    void slotSendMessage(const QString &msg);
    void slotTextEditing(bool clearNotification);
    void sendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo);
    void slotSendFile();
    void keyPressedInLineEdit(QKeyEvent *ev);
    void textEditClicked();
    Q_REQUIRED_RESULT MessageModel *messageModel() const;
    void clearEditingMode();

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
    EmoticonMenuWidget *mEmoticonMenuWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    bool mReplyInThreadDialogBox = false;
};

