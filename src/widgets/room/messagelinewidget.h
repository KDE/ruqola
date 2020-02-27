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

#ifndef MESSAGELINEWIDGET_H
#define MESSAGELINEWIDGET_H

#include <QPointer>
#include <QWidget>
#include "dialogs/uploadfiledialog.h"
#include "libruqolawidgets_private_export.h"

class MessageTextEdit;
class QToolButton;
class EmoticonMenuWidget;
class RocketChatAccount;
class QMimeData;

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

    void setCurrentRocketChatAccount(RocketChatAccount *account);

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

    bool handleMimeData(const QMimeData *mimeData);

    void clearMessageIdBeingEdited();

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

private:
    void slotSendMessage(const QString &msg);
    void slotTextEditing(bool clearNotification);
    void sendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo);
    void slotSendFile();

    QString mRoomId;
    QString mThreadMessageId;
    QString mMessageIdBeingEdited;
    EditingMode mMode = EditingMode::NewMessage;
    MessageTextEdit *mMessageTextEdit = nullptr;
    QToolButton *mSendFile = nullptr;
    QToolButton *mEmoticonButton = nullptr;
    QToolButton *mSendMessageButton = nullptr;
    EmoticonMenuWidget *mEmoticonMenuWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};

#endif // MESSAGELINEWIDGET_H
