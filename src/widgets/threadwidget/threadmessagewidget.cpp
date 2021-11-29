/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

ThreadMessageWidget::ThreadMessageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mThreadPreview(new QLabel(this))
    , mRoomWidgetBase(new RoomWidgetBase(MessageListView::Mode::ThreadEditing, this))
    , mRocketChatAccount(account)
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
    setAcceptDrops(true);
    if (mRocketChatAccount) {
        intialize();
    }
}

ThreadMessageWidget::~ThreadMessageWidget() = default;

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
        mRocketChatAccount->getThreadMessages(mThreadMessageId);
        mRoomWidgetBase->messageListView()->setModel(mRocketChatAccount->threadMessageModel());
        mRoomWidgetBase->messageLineWidget()->setThreadMessageId(mThreadMessageId, {}, true);
    }
}

void ThreadMessageWidget::intialize()
{
    mRoomWidgetBase->setCurrentRocketChatAccount(mRocketChatAccount);
    mRoomWidgetBase->messageLineWidget()->setCurrentRocketChatAccount(mRocketChatAccount, true);
    mRoomWidgetBase->messageListView()->setCurrentRocketChatAccount(mRocketChatAccount);
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
