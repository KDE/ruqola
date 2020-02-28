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

#include "messagelinewidget.h"
#include "messagetextedit.h"
#include "misc/emoticonmenuwidget.h"
#include "dialogs/uploadfiledialog.h"
#include "rocketchataccount.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QPointer>
#include <QHBoxLayout>
#include <QToolButton>
#include <QMenu>
#include <QWidgetAction>
#include <QMimeData>
#include <QDir>
#include <QImageWriter>
#include <QTemporaryFile>

MessageLineWidget::MessageLineWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mSendFile = new QToolButton(this);
    mSendFile->setObjectName(QStringLiteral("mSendFile"));
    mainLayout->addWidget(mSendFile);
    mSendFile->setIcon(QIcon::fromTheme(QStringLiteral("document-send-symbolic")));
    connect(mSendFile, &QToolButton::clicked, this, &MessageLineWidget::slotSendFile);

    mMessageTextEdit = new MessageTextEdit(this);
    mMessageTextEdit->setObjectName(QStringLiteral("mMessageTextEdit"));
    mainLayout->addWidget(mMessageTextEdit);
    connect(mMessageTextEdit, &MessageTextEdit::sendMessage, this, &MessageLineWidget::slotSendMessage);
    connect(mMessageTextEdit, &MessageTextEdit::keyPressed, this, &MessageLineWidget::keyPressedInLineEdit);

    mEmoticonButton = new QToolButton(this);
    mEmoticonButton->setObjectName(QStringLiteral("mEmoticonButton"));
    mEmoticonButton->setIcon(QIcon::fromTheme(QStringLiteral("face-smile")));
    mEmoticonButton->setPopupMode(QToolButton::InstantPopup);
    mainLayout->addWidget(mEmoticonButton);

    mSendMessageButton = new QToolButton(this);
    mSendMessageButton->setObjectName(QStringLiteral("mSendMessageButton"));
    mSendMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("mail-sent")));
    mainLayout->addWidget(mSendMessageButton);
    connect(mSendMessageButton, &QToolButton::clicked, this, [this]() {
        slotSendMessage(mMessageTextEdit->text());
        mMessageTextEdit->clear();
    });

    auto *emoticonMenu = new QMenu(this);
    auto *action = new QWidgetAction(emoticonMenu);
    mEmoticonMenuWidget = new EmoticonMenuWidget(this);
    action->setDefaultWidget(mEmoticonMenuWidget);
    emoticonMenu->addAction(action);
    mEmoticonButton->setMenu(emoticonMenu);
    connect(mEmoticonMenuWidget, &EmoticonMenuWidget::insertEmoticons, mMessageTextEdit, &MessageTextEdit::insertEmoji);

    setFocusProxy(mMessageTextEdit);
}

MessageLineWidget::~MessageLineWidget()
{
}

void MessageLineWidget::slotSendMessage(const QString &msg)
{
    if (!msg.isEmpty()) {
        if (mMessageIdBeingEdited.isEmpty()) {
            if (mThreadMessageId.isEmpty()) {
                mCurrentRocketChatAccount->sendMessage(mRoomId, msg);
            } else {
                mCurrentRocketChatAccount->replyOnThread(mRoomId, mThreadMessageId, msg);
            }
        } else {
            //TODO support mThreadMessageId too
            mCurrentRocketChatAccount->updateMessage(mRoomId, mMessageIdBeingEdited, msg);
            mMessageIdBeingEdited.clear();
        }
        setMode(MessageLineWidget::EditingMode::NewMessage);
    }
}

void MessageLineWidget::setEditMessage(const QString &messageId, const QString &text)
{
    mMessageIdBeingEdited = messageId;
    setMode(messageId.isEmpty() ? MessageLineWidget::EditingMode::NewMessage : MessageLineWidget::EditingMode::EditMessage);
    setText(text);
    setFocus();
}

void MessageLineWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
    mMessageTextEdit->setCurrentRocketChatAccount(account);
    mEmoticonMenuWidget->setCurrentRocketChatAccount(account);
}

void MessageLineWidget::setText(const QString &text)
{
    mMessageTextEdit->setPlainText(text);

    QTextCursor cursor(mMessageTextEdit->document());
    cursor.movePosition(QTextCursor::End);
    mMessageTextEdit->setTextCursor(cursor);
}

QString MessageLineWidget::text() const
{
    return mMessageTextEdit->text();
}

MessageTextEdit *MessageLineWidget::messageTextEdit() const
{
    return mMessageTextEdit;
}

void MessageLineWidget::slotSendFile()
{
    QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
    if (dlg->exec()) {
        const UploadFileDialog::UploadFileInfo result = dlg->fileInfo();
        sendFile(result);
    }
    delete dlg;
}

QString MessageLineWidget::threadMessageId() const
{
    return mThreadMessageId;
}

void MessageLineWidget::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

MessageLineWidget::EditingMode MessageLineWidget::mode() const
{
    return mMode;
}

void MessageLineWidget::setMode(EditingMode mode)
{
    if (mMode != mode) {
        mMode = mode;
        switch (mMode) {
        case EditingMode::NewMessage:
            mSendMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("mail-sent")));
            break;
        case EditingMode::EditMessage:
            mSendMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-symbolic")));
            break;
        }
    }
}

void MessageLineWidget::slotTextEditing(bool clearNotification)
{
    mCurrentRocketChatAccount->textEditing(mRoomId, clearNotification);
}

void MessageLineWidget::sendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo)
{
    mCurrentRocketChatAccount->uploadFile(mRoomId, uploadFileInfo.description, QString(), uploadFileInfo.fileUrl, mThreadMessageId);
}

QString MessageLineWidget::messageIdBeingEdited() const
{
    return mMessageIdBeingEdited;
}

void MessageLineWidget::setMessageIdBeingEdited(const QString &messageIdBeingEdited)
{
    mMessageIdBeingEdited = messageIdBeingEdited;
}

QString MessageLineWidget::roomId() const
{
    return mRoomId;
}

void MessageLineWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool MessageLineWidget::handleMimeData(const QMimeData *mimeData)
{
    auto uploadFile = [this](const QUrl &url) {
                          QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
                          dlg->setFileUrl(url);
                          if (dlg->exec()) {
                              const UploadFileDialog::UploadFileInfo uploadFileInfo = dlg->fileInfo();
                              sendFile(uploadFileInfo);
                          }
                      };
    if (mimeData->hasUrls()) {
        const QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls) {
            if (url.isLocalFile()) {
                uploadFile(url);
            }
        }
        return true;
    } else if (mimeData->hasImage()) {
        QTemporaryFile tempFile(QDir::tempPath() + QLatin1String("/XXXXXX.png"));
        if (tempFile.open()) {
            QImage image = mimeData->imageData().value<QImage>();
            QImageWriter writer(&tempFile, "PNG");
            if (writer.write(image)) {
                const QUrl url = QUrl::fromLocalFile(tempFile.fileName());
                tempFile.close();
                uploadFile(url);
                return true;
            }
        }
    }
    return false;
}

void MessageLineWidget::clearMessageIdBeingEdited()
{
    mMessageIdBeingEdited.clear();
    setText(QString());
    setMode(MessageLineWidget::EditingMode::NewMessage);
}

void MessageLineWidget::keyPressedInLineEdit(QKeyEvent *ev)
{
    const int key = ev->key();
    if (key == Qt::Key_Escape) {
        if (!mMessageIdBeingEdited.isEmpty()) {
            clearMessageIdBeingEdited();
            ev->accept();
        } else {
            Q_EMIT keyPressed(ev);
        }
    } else if (ev->matches(QKeySequence::Paste)) {
        const QMimeData *mimeData = qApp->clipboard()->mimeData();
        if (handleMimeData(mimeData)) {
            ev->accept();
        }
    } else if ((key == Qt::Key_Up || key == Qt::Key_Down) && ev->modifiers() & Qt::AltModifier) {
        MessageModel *model = mThreadMessageId.isEmpty() ? mCurrentRocketChatAccount->messageModelForRoom(mRoomId) : mCurrentRocketChatAccount->threadMessageModel();
        Q_ASSERT(model);
        auto isEditable = [this](const Message &msg) {
                              return mCurrentRocketChatAccount->isMessageEditable(msg);
                          };
        if (key == Qt::Key_Up) {
            const Message &msg = model->findLastMessageBefore(mMessageIdBeingEdited, isEditable);
            setEditMessage(msg.messageId(), msg.text());
        } else {
            const Message &msg = model->findNextMessageAfter(mMessageIdBeingEdited, isEditable);
            setEditMessage(msg.messageId(), msg.text());
        }
        ev->accept();
    } else {
        Q_EMIT keyPressed(ev);
    }
}
