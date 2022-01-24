/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomwidgetbase.h"
#include "dialogs/createnewdiscussiondialog.h"
#include "messagelinewidget.h"
#include "messagetextedit.h"
#include "readonlylineeditwidget.h"
#include "rocketchataccount.h"
#include "room.h"
#include "roomquotemessagewidget.h"
#include "roomreplythreadwidget.h"
#include "roomutil.h"
#include "uploadfileprogressstatuswidget.h"

#include <QKeyEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

RoomWidgetBase::RoomWidgetBase(MessageListView::Mode mode, QWidget *parent)
    : QWidget(parent)
    , mUploadFileProgressStatusWidget(new UploadFileProgressStatusWidget(this))
    , mMessageListView(new MessageListView(mode, this))
    , mRoomReplyThreadWidget(new RoomReplyThreadWidget(this))
    , mRoomQuoteMessageWidget(new RoomQuoteMessageWidget(this))
    , mStackedWidget(new QStackedWidget(this))
    , mMessageLineWidget(new MessageLineWidget(this))
    , mReadOnlyLineEditWidget(new ReadOnlyLineEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mUploadFileProgressStatusWidget->setObjectName(QStringLiteral("mUploadFileProgressStatusWidget"));
    mUploadFileProgressStatusWidget->setVisible(false);
    mainLayout->addWidget(mUploadFileProgressStatusWidget);

    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    mainLayout->addWidget(mMessageListView, 1);

    mRoomReplyThreadWidget->setObjectName(QStringLiteral("mRoomReplyThreadWidget"));
    mRoomReplyThreadWidget->setVisible(false);

    connect(mRoomReplyThreadWidget, &RoomReplyThreadWidget::cancelReplyingInThread, this, [this] {
        mMessageLineWidget->setThreadMessageId({});
    });

    mRoomQuoteMessageWidget->setObjectName(QStringLiteral("mRoomQuoteMessageWidget"));
    mRoomQuoteMessageWidget->setVisible(false);

    connect(mRoomQuoteMessageWidget, &RoomQuoteMessageWidget::cancelQuoteMessage, this, [this] {
        mMessageLineWidget->setQuoteMessage({}, QString());
    });

    mainLayout->addWidget(mRoomReplyThreadWidget);
    mainLayout->addWidget(mRoomQuoteMessageWidget);

    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mMessageLineWidget->setObjectName(QStringLiteral("mMessageLineWidget"));
    mStackedWidget->addWidget(mMessageLineWidget);

    mReadOnlyLineEditWidget->setObjectName(QStringLiteral("mReadOnlyLineEditWidget"));
    mStackedWidget->addWidget(mReadOnlyLineEditWidget);

    mStackedWidget->setCurrentWidget(mMessageLineWidget);

    connect(mMessageLineWidget, &MessageLineWidget::keyPressed, this, &RoomWidgetBase::keyPressedInLineEdit);
    if (mode == MessageListView::Mode::Editing) {
        connect(mMessageLineWidget, &MessageLineWidget::threadMessageIdChanged, this, &RoomWidgetBase::slotShowThreadMessage);
    }
    connect(mMessageLineWidget, &MessageLineWidget::quoteMessageChanged, this, &RoomWidgetBase::slotShowQuoteMessage);

    connect(mMessageListView, &MessageListView::editMessageRequested, mMessageLineWidget, &MessageLineWidget::setEditMessage);
    connect(mMessageListView, &MessageListView::quoteMessageRequested, mMessageLineWidget, &MessageLineWidget::setQuoteMessage);
    connect(mMessageListView, &MessageListView::createNewDiscussion, this, &RoomWidgetBase::createNewDiscussion);
    connect(mMessageListView, &MessageListView::createPrivateConversation, this, &RoomWidgetBase::slotCreatePrivateDiscussion);
    connect(mMessageListView, &MessageListView::loadHistoryRequested, this, &RoomWidgetBase::loadHistory);
    connect(mMessageListView, &MessageListView::replyInThreadRequested, mMessageLineWidget, [this](const QString &messageId, const QString &text) {
        mMessageLineWidget->setThreadMessageId(messageId, text, false);
    });

    setAcceptDrops(true);
}

RoomWidgetBase::~RoomWidgetBase() = default;

void RoomWidgetBase::slotShowThreadMessage(const QString &threadMessageId, const QString &text)
{
    mRoomReplyThreadWidget->setMessageText(text);
    mRoomReplyThreadWidget->setVisible(!threadMessageId.isEmpty());
}

MessageListView *RoomWidgetBase::messageListView() const
{
    return mMessageListView;
}

MessageLineWidget *RoomWidgetBase::messageLineWidget() const
{
    return mMessageLineWidget;
}

void RoomWidgetBase::slotShowQuoteMessage(const QString &permalink, const QString &text)
{
    mRoomQuoteMessageWidget->setMessageText(text);
    mRoomQuoteMessageWidget->setVisible(!permalink.isEmpty());
}

void RoomWidgetBase::slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage, const QString &channelName)
{
    CreateNewDiscussionDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setDiscussionName(originalMessage);
    dlg.setChannelInfo(channelName, mRoomId);
    dlg.setMessageId(messageId);
    dlg.exec();
}

void RoomWidgetBase::closeQuoteAndThreadInfo()
{
    mRoomReplyThreadWidget->setVisible(false);
    mRoomQuoteMessageWidget->setVisible(false);
}

void RoomWidgetBase::slotCreatePrivateDiscussion(const QString &userName)
{
    Q_EMIT mCurrentRocketChatAccount->openLinkRequested(RoomUtil::generateUserLink(userName));
}

void RoomWidgetBase::keyPressedInLineEdit(QKeyEvent *ev)
{
    const int key = ev->key();
    if (key == Qt::Key_Escape) {
        slotClearNotification();
        ev->accept();
    } else if (ev->matches(QKeySequence::Copy) && mMessageLineWidget->messageTextEdit()->textCursor().selectedText().isEmpty()) {
        mMessageListView->copyMessageToClipboard();
        ev->accept();
    } else {
        mMessageListView->handleKeyPressEvent(ev);
    }
}

void RoomWidgetBase::slotUploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info)
{
    if (info.bytesSent > 0 && info.bytesTotal > 0) {
        mUploadFileProgressStatusWidget->setVisible(true);
        mUploadFileProgressStatusWidget->setUploadFileName(info.fileName);
        mUploadFileProgressStatusWidget->setValue(static_cast<int>((info.bytesSent * 100) / info.bytesTotal));
    } else {
        mUploadFileProgressStatusWidget->setVisible(false);
    }
}

QString RoomWidgetBase::roomId() const
{
    return mRoomId;
}

void RoomWidgetBase::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void RoomWidgetBase::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged, mMessageLineWidget, &MessageLineWidget::slotPublicSettingChanged);
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::uploadProgress, this, &RoomWidgetBase::slotUploadProgress);
        disconnect(mCurrentRocketChatAccount,
                   &RocketChatAccount::ownUserPreferencesChanged,
                   mMessageLineWidget,
                   &MessageLineWidget::slotOwnUserPreferencesChanged);
        // hide it when we switch account.
        mUploadFileProgressStatusWidget->setVisible(false);
    }

    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged, mMessageLineWidget, &MessageLineWidget::slotPublicSettingChanged);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::uploadProgress, this, &RoomWidgetBase::slotUploadProgress);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::ownUserPreferencesChanged, mMessageLineWidget, &MessageLineWidget::slotOwnUserPreferencesChanged);
    mMessageListView->setCurrentRocketChatAccount(account);
    mMessageLineWidget->setCurrentRocketChatAccount(account, false);
    // When we switch we need to update it.
    mMessageLineWidget->slotPublicSettingChanged();
    mMessageLineWidget->slotOwnUserPreferencesChanged();
    mRoomId.clear(); // Clear it otherwise if we switch between two account with same roomId (as "GENERAL") we will see incorrect room.
}

void RoomWidgetBase::slotClearNotification()
{
    if (!mRoomId.isEmpty()) {
        mCurrentRocketChatAccount->markRoomAsRead(mRoomId);
    }
}

void RoomWidgetBase::updateListView()
{
    mMessageListView->clearTextDocumentCache();
    mMessageListView->viewport()->update();
}

void RoomWidgetBase::updateRoomReadOnly(Room *room)
{
    if (room->roomMessageInfo().isEmpty()) {
        mStackedWidget->setCurrentWidget(mMessageLineWidget);
    } else {
        mStackedWidget->setCurrentWidget(mReadOnlyLineEditWidget);
        mReadOnlyLineEditWidget->setMessage(room->roomMessageInfo());
    }
}
