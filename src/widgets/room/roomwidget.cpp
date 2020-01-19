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
#include <QHBoxLayout>
#include <KLocalizedString>
#include "ruqola.h"
#include "rocketchataccount.h"
#include <roomwrapper.h>

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

    mMessageLineWidget = new MessageLineWidget(this);
    mMessageLineWidget->setObjectName(QStringLiteral("mMessageLineWidget"));
    mainLayout->addWidget(mMessageLineWidget);

    connect(this, &RoomWidget::channelSelected, this, &RoomWidget::setChannelSelected);
    connect(mMessageLineWidget, &MessageLineWidget::sendMessage, this, &RoomWidget::slotSendMessage);
    connect(mMessageLineWidget, &MessageLineWidget::clearNotification, this, &RoomWidget::slotClearNotification);
    connect(mMessageLineWidget, &MessageLineWidget::sendFile, this, &RoomWidget::slotSendFile);
}

RoomWidget::~RoomWidget()
{
    delete mRoomWrapper;
}

void RoomWidget::slotSendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo)
{
    Ruqola::self()->rocketChatAccount()->uploadFile(mRoomId, uploadFileInfo.description, uploadFileInfo.message, uploadFileInfo.fileUrl);
}

void RoomWidget::slotSendMessage(const QString &msg)
{
    Ruqola::self()->rocketChatAccount()->sendMessage(mRoomId, msg);
}

void RoomWidget::setChannelSelected(const QModelIndex &index)
{
    setRoomId(index.data(RoomModel::RoomID).toString());
    //Use roomwrapper here!
    //Description ???
}

void RoomWidget::updateRoomHeader()
{
    if (mRoomWrapper) {
        mRoomHeaderWidget->setRoomName(mRoomWrapper->name());
        mRoomHeaderWidget->setRoomAnnouncement(mRoomWrapper->announcement());
        mRoomHeaderWidget->setRoomTopic(mRoomWrapper->topic());
    }
}

QString RoomWidget::roomId() const
{
    return mRoomId;
}

void RoomWidget::setRoomId(const QString &roomId)
{
    if (mRoomId != roomId) {
        mRoomId = roomId;
        mMessageListView->setChannelSelected(roomId);
        delete mRoomWrapper;
        mRoomWrapper = Ruqola::self()->rocketChatAccount()->roomWrapper(mRoomId);
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
        updateRoomHeader();
    }
}

void RoomWidget::slotClearNotification()
{
    Ruqola::self()->rocketChatAccount()->clearUnreadMessages(mRoomId);
}
