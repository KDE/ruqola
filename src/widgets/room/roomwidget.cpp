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

#include "roomwidget.h"
#include "roomheaderwidget.h"
#include "messagelistview.h"
#include "messagelinewidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "roomwrapper.h"
#include "readonlylineeditwidget.h"
#include "messagelineedit.h"

#include <KLocalizedString>

#include <QKeyEvent>
#include <QHBoxLayout>
#include <QApplication>
#include <QStackedWidget>

RoomWidget::RoomWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mRoomHeaderWidget = new RoomHeaderWidget(this);
    mRoomHeaderWidget->setObjectName(QStringLiteral("mRoomHeaderWidget"));
    mainLayout->addWidget(mRoomHeaderWidget);

    mMessageListView = new MessageListView(this);
    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    mainLayout->addWidget(mMessageListView);

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mMessageLineWidget = new MessageLineWidget(this);
    mMessageLineWidget->setObjectName(QStringLiteral("mMessageLineWidget"));
    mStackedWidget->addWidget(mMessageLineWidget);

    mReadOnlyLineEditWidget = new ReadOnlyLineEditWidget(this);
    mReadOnlyLineEditWidget->setObjectName(QStringLiteral("mReadOnlyLineEditWidget"));
    mStackedWidget->addWidget(mReadOnlyLineEditWidget);

    mStackedWidget->setCurrentWidget(mMessageLineWidget);
    mStackedWidget->setMaximumHeight(mMessageLineWidget->height());

    connect(this, &RoomWidget::channelSelected, this, &RoomWidget::setChannelSelected);
    connect(mMessageLineWidget, &MessageLineWidget::sendMessage, this, &RoomWidget::slotSendMessage);
    connect(mMessageLineWidget, &MessageLineWidget::sendFile, this, &RoomWidget::slotSendFile);
    connect(mMessageLineWidget->messageLineEdit(), &MessageLineEdit::keyPressed, this, &RoomWidget::keyPressedInLineEdit);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::favoriteChanged, this, &RoomWidget::slotChangeFavorite);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::encryptedChanged, this, &RoomWidget::slotEncryptedChanged);

    connect(mMessageListView, &MessageListView::editMessageRequested, this, &RoomWidget::slotEditMessage);
}

RoomWidget::~RoomWidget()
{
    delete mRoomWrapper;
}

void RoomWidget::slotSendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo)
{
    mCurrentRocketChatAccount->uploadFile(mRoomId, uploadFileInfo.description, QString(), uploadFileInfo.fileUrl);
}

void RoomWidget::slotSendMessage(const QString &msg)
{
    if (mMessageIdBeingEdited.isEmpty()) {
        mCurrentRocketChatAccount->sendMessage(mRoomId, msg);
    } else {
        mCurrentRocketChatAccount->updateMessage(mRoomId, mMessageIdBeingEdited, msg);
        mMessageIdBeingEdited.clear();
    }
}

void RoomWidget::slotEditMessage(const QString &messageId, const QString &text)
{
    mMessageIdBeingEdited = messageId;
    mMessageLineWidget->setText(text);
    mMessageLineWidget->setFocus();
}

void RoomWidget::setChannelSelected(const QModelIndex &index)
{
    setRoomId(index.data(RoomModel::RoomID).toString());
    setRoomType(index.data(RoomModel::RoomType).toString());
    mMessageLineWidget->setFocus();
}

void RoomWidget::updateRoomHeader()
{
    if (mRoomWrapper) {
        mRoomHeaderWidget->setRoomName(mRoomWrapper->name());
        mRoomHeaderWidget->setRoomAnnouncement(mRoomWrapper->announcement());
        mRoomHeaderWidget->setRoomTopic(mRoomWrapper->topic());
        mRoomHeaderWidget->setFavoriteStatus(mRoomWrapper->favorite());
        mRoomHeaderWidget->setEncypted(mRoomWrapper->encrypted());
        //TODO Description ?

        if (mRoomWrapper->readOnly()) {
            mStackedWidget->setCurrentWidget(mReadOnlyLineEditWidget);
        } else {
            mStackedWidget->setCurrentWidget(mMessageLineWidget);
        }
    } else {
        //Hide it
    }
}

QString RoomWidget::roomId() const
{
    return mRoomId;
}

void RoomWidget::setRoomType(const QString &roomType)
{
    mRoomType = roomType;
}

RoomWrapper *RoomWidget::roomWrapper() const
{
    return mRoomWrapper;
}

void RoomWidget::setRoomId(const QString &roomId)
{
    if (mRoomId != roomId) {
        mRoomId = roomId;
        mMessageListView->setChannelSelected(roomId);
        delete mRoomWrapper;
        mRoomWrapper = mCurrentRocketChatAccount->roomWrapper(mRoomId);
        connectRoomWrapper();
    }
}

void RoomWidget::connectRoomWrapper()
{
    if (mRoomWrapper) {
        connect(mRoomWrapper, &RoomWrapper::announcementChanged, this, [this]() {
            mRoomHeaderWidget->setRoomAnnouncement(mRoomWrapper->announcement());
        });
        connect(mRoomWrapper, &RoomWrapper::topicChanged, this, [this]() {
            mRoomHeaderWidget->setRoomTopic(mRoomWrapper->topic());
        });
        connect(mRoomWrapper, &RoomWrapper::nameChanged, this, [this]() {
            mRoomHeaderWidget->setRoomName(mRoomWrapper->name());
        });
        connect(mRoomWrapper, &RoomWrapper::favoriteChanged, this, [this]() {
            mRoomHeaderWidget->setFavoriteStatus(mRoomWrapper->favorite());
        });
        connect(mRoomWrapper, &RoomWrapper::encryptedChanged, this, [this]() {
            mRoomHeaderWidget->setEncypted(mRoomWrapper->encrypted());
        });
        updateRoomHeader();
    }
}

void RoomWidget::slotClearNotification()
{
    mCurrentRocketChatAccount->clearUnreadMessages(mRoomId);
}

void RoomWidget::slotEncryptedChanged(bool b)
{
    //TODO mCurrentRocketChatAccount->slot
}

void RoomWidget::slotChangeFavorite(bool b)
{
    mCurrentRocketChatAccount->changeFavorite(mRoomId, b);
}

void RoomWidget::keyPressedInLineEdit(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Escape) {
        if (!mMessageIdBeingEdited.isEmpty()) {
            mMessageIdBeingEdited.clear();
            mMessageLineWidget->setText(QString());
        } else {
            slotClearNotification();
        }
        ev->accept();
    } else {
        mMessageListView->handleKeyPressEvent(ev);
    }
}

QString RoomWidget::roomType() const
{
    return mRoomType;
}

void RoomWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
    mMessageLineWidget->setCurrentRocketChatAccount(account);
}
