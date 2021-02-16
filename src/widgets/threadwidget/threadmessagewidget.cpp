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

#include "threadmessagewidget.h"
#include "rocketchataccount.h"
#include "room.h"
#include "room/messagelinewidget.h"
#include "room/messagelistview.h"
#include "room/messagetextedit.h"
#include "room/roomwidgetbase.h"
#include "ruqola.h"
#include <QLabel>
#include <QMimeData>
#include <QVBoxLayout>

ThreadMessageWidget::ThreadMessageWidget(QWidget *parent)
    : QWidget(parent)
    , mThreadPreview(new QLabel(this))
    , mRoomWidgetBase(new RoomWidgetBase(MessageListView::Mode::ThreadEditing, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mThreadPreview->setObjectName(QStringLiteral("mThreadPreview"));
    mThreadPreview->setContextMenuPolicy(Qt::CustomContextMenu);
    mThreadPreview->setWordWrap(true);
    mainLayout->addWidget(mThreadPreview);

    mRoomWidgetBase->setObjectName(QStringLiteral("mRoomWidgetBase"));
    mainLayout->addWidget(mRoomWidgetBase);
    connect(mRoomWidgetBase, &RoomWidgetBase::createNewDiscussion, this, &ThreadMessageWidget::slotCreateNewDiscussion);
}

ThreadMessageWidget::~ThreadMessageWidget()
{
}

void ThreadMessageWidget::slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage)
{
    mRoomWidgetBase->slotCreateNewDiscussion(messageId, originalMessage, QString());
}

QString ThreadMessageWidget::threadMessageId() const
{
    return mThreadMessageId;
}

void ThreadMessageWidget::setThreadMessageId(const QString &threadMessageId)
{
    if (mThreadMessageId != threadMessageId) {
        mThreadMessageId = threadMessageId;
        Ruqola::self()->rocketChatAccount()->getThreadMessages(mThreadMessageId);
        mRoomWidgetBase->messageListView()->setModel(Ruqola::self()->rocketChatAccount()->threadMessageModel());
        mRoomWidgetBase->messageLineWidget()->setThreadMessageId(mThreadMessageId, true);
    }
}

void ThreadMessageWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mRoomWidgetBase->setCurrentRocketChatAccount(account);
    mRoomWidgetBase->messageLineWidget()->setCurrentRocketChatAccount(account, true);
    mRoomWidgetBase->messageListView()->setCurrentRocketChatAccount(account);
    // When we switch we need to update it.
    mRoomWidgetBase->messageLineWidget()->slotPublicSettingChanged();
    mRoomWidgetBase->messageLineWidget()->slotOwnUserPreferencesChanged();
}

void ThreadMessageWidget::setRoom(Room *room)
{
    mRoomWidgetBase->messageLineWidget()->setRoomId(room->roomId());
    mRoomWidgetBase->messageListView()->setRoom(room);
}

void ThreadMessageWidget::setThreadPreview(const QString &preview)
{
    mThreadPreview->setText(preview);
}

void ThreadMessageWidget::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        event->accept();
    }
}

void ThreadMessageWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        mRoomWidgetBase->messageLineWidget()->handleMimeData(mimeData);
    }
}
