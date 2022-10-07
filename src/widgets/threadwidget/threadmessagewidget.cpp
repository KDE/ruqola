/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagewidget.h"
#include "chat/followmessagejob.h"
#include "chat/unfollowmessagejob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "room.h"
#include "room/messagelinewidget.h"
#include "room/messagelistview.h"
#include "room/roomwidgetbase.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QMimeData>
#include <QToolButton>
#include <QVBoxLayout>

ThreadMessageWidget::ThreadMessageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mThreadPreview(new QLabel(this))
    , mRoomWidgetBase(new RoomWidgetBase(MessageListView::Mode::ThreadEditing, this))
    , mRocketChatAccount(account)
    , mFollowButton(new QToolButton(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins(QMargins());

    mainLayout->addLayout(hboxLayout);

    mFollowButton->setObjectName(QStringLiteral("mFollowButton"));
    mFollowButton->setCheckable(true);
    mFollowButton->setAutoRaise(true);
    hboxLayout->addWidget(mFollowButton);
    connect(mFollowButton, &QToolButton::clicked, this, &ThreadMessageWidget::slotFollowThreadChanged);

    mThreadPreview->setObjectName(QStringLiteral("mThreadPreview"));
    mThreadPreview->setContextMenuPolicy(Qt::CustomContextMenu);
    mThreadPreview->setWordWrap(true);
    hboxLayout->addWidget(mThreadPreview);

    mRoomWidgetBase->setObjectName(QStringLiteral("mRoomWidgetBase"));
    mainLayout->addWidget(mRoomWidgetBase);
    connect(mRoomWidgetBase, &RoomWidgetBase::createNewDiscussion, this, &ThreadMessageWidget::slotCreateNewDiscussion);
    setAcceptDrops(true);
    if (mRocketChatAccount) {
        initialize();
    }
}

ThreadMessageWidget::~ThreadMessageWidget() = default;

void ThreadMessageWidget::slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage)
{
    mRoomWidgetBase->slotCreateNewDiscussion(messageId, originalMessage, QString());
}

void ThreadMessageWidget::slotFollowThreadChanged(bool clicked)
{
    if (clicked) {
        auto job = new RocketChatRestApi::UnFollowMessageJob(this);
        job->setMessageId(mThreadMessageId);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::UnFollowMessageJob::unFollowMessageDone, this, [this]() {
            updateFollowThreadIcon(false); // TODO verify it
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UnFollowMessageJob job";
        }
    } else {
        auto job = new RocketChatRestApi::FollowMessageJob(this);
        job->setMessageId(mThreadMessageId);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::FollowMessageJob::followMessageDone, this, [this]() {
            updateFollowThreadIcon(true); // TODO verify it
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start FollowMessageJob job";
        }
    }
}

void ThreadMessageWidget::updateFollowThreadIcon(bool followThread)
{
    mFollowButton->setIcon(followThread ? QIcon::fromTheme(QStringLiteral("notifications")) : QIcon::fromTheme(QStringLiteral("notifications-disabled")));
    mFollowButton->setToolTip(followThread ? i18n("Follow Message") : i18n("Unfollow Message"));
}

QString ThreadMessageWidget::threadMessageId() const
{
    return mThreadMessageId;
}

void ThreadMessageWidget::setFollowingThread(bool threadIsFollowing)
{
    updateFollowThreadIcon(threadIsFollowing);
    mFollowButton->setChecked(!threadIsFollowing);
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

void ThreadMessageWidget::initialize()
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
    mRoom = room;
    mRoomWidgetBase->messageLineWidget()->setRoomId(room->roomId());
    mRoomWidgetBase->messageListView()->setRoom(room);
    mRoomWidgetBase->updateRoomReadOnly(room);
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
    // Don't allow to drop element when it's blocked
    if (mRoom && mRoom->roomIsBlocked()) {
        return;
    }
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        mRoomWidgetBase->messageLineWidget()->handleMimeData(mimeData);
    }
}
