/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagewidget.h"
using namespace Qt::Literals::StringLiterals;

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

#include "config-ruqola.h"

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

ThreadMessageWidget::ThreadMessageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mThreadPreview(new QLabel(this))
    , mRoomWidgetBase(new RoomWidgetBase(MessageListView::Mode::ThreadEditing, this))
    , mRocketChatAccount(account)
    , mFollowButton(new QToolButton(this))
#if HAVE_TEXT_TO_SPEECH
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(u"hboxLayout"_s);
    hboxLayout->setContentsMargins(QMargins());

    mainLayout->addLayout(hboxLayout);

    mFollowButton->setObjectName(u"mFollowButton"_s);
    mFollowButton->setCheckable(true);
    mFollowButton->setAutoRaise(true);
    hboxLayout->addWidget(mFollowButton);
    connect(mFollowButton, &QToolButton::clicked, this, &ThreadMessageWidget::slotFollowThreadChanged);

    mThreadPreview->setObjectName(u"mThreadPreview"_s);
    mThreadPreview->setContextMenuPolicy(Qt::NoContextMenu);
    mThreadPreview->setWordWrap(true);
    mThreadPreview->setTextInteractionFlags(Qt::TextBrowserInteraction);
    hboxLayout->addWidget(mThreadPreview);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mRoomWidgetBase, &RoomWidgetBase::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::enqueue);
#endif

    mRoomWidgetBase->setObjectName(u"mRoomWidgetBase"_s);
    mainLayout->addWidget(mRoomWidgetBase);
    mRoomWidgetBase->messageLineWidget()->setFocus();
    connect(mRoomWidgetBase, &RoomWidgetBase::createNewDiscussion, this, &ThreadMessageWidget::slotCreateNewDiscussion);
    setAcceptDrops(true);
    if (mRocketChatAccount) {
        initialize();
    }
}

ThreadMessageWidget::~ThreadMessageWidget() = default;

void ThreadMessageWidget::slotCreateNewDiscussion(const QByteArray &messageId, const QString &originalMessage)
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
    mFollowButton->setIcon(followThread ? QIcon::fromTheme(u"notifications"_s) : QIcon::fromTheme(u"notifications-disabled"_s));
    mFollowButton->setToolTip(followThread ? i18n("Follow Message") : i18n("Unfollow Message"));
}

void ThreadMessageWidget::setThreadMessageInfo(const ThreadMessageWidget::ThreadMessageInfo &info)
{
    updateFollowThreadIcon(info.threadIsFollowing);
    mFollowButton->setChecked(!info.threadIsFollowing);

    mRoom = info.room;
    if (mRoom) {
        mRoomWidgetBase->messageLineWidget()->setRoomId(mRoom->roomId());
        mRoomWidgetBase->messageListView()->setRoom(mRoom);
        mRoomWidgetBase->updateRoomReadOnly(mRoom);
        mRoomWidgetBase->messageLineWidget()->setRoomName(mRoom->displayRoomName(), true);
    }
    mThreadPreview->setText(info.threadMessagePreview);
    if (mThreadMessageId != info.threadMessageId) {
        mThreadMessageId = info.threadMessageId;
        mRocketChatAccount->getThreadMessages(mThreadMessageId, info.messageThread);
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

#include "moc_threadmessagewidget.cpp"
