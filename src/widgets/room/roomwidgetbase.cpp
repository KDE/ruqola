/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
#include "uploadfilemanager.h"
#include "uploadfileprogressstatuslistwidget.h"

#include <QKeyEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

RoomWidgetBase::RoomWidgetBase(MessageListView::Mode mode, QWidget *parent)
    : QWidget(parent)
    , mUploadFileProgressStatusListWidget(new UploadFileProgressStatusListWidget(this))
    , mMessageListView(new MessageListView(nullptr, mode, this))
    , mRoomReplyThreadWidget(new RoomReplyThreadWidget(this))
    , mRoomQuoteMessageWidget(new RoomQuoteMessageWidget(this))
    , mStackedWidget(new QStackedWidget(this))
    , mMessageLineWidget(new MessageLineWidget(this))
    , mReadOnlyLineEditWidget(new ReadOnlyLineEditWidget(this))
    , mMainLayout(new QVBoxLayout(this))
{
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    mMainLayout->setSpacing(0);
#endif

    mUploadFileProgressStatusListWidget->setObjectName(QStringLiteral("mUploadFileProgressStatusListWidget"));
    mUploadFileProgressStatusListWidget->setVisible(false);
    mMainLayout->addWidget(mUploadFileProgressStatusListWidget);
    connect(mUploadFileProgressStatusListWidget, &UploadFileProgressStatusListWidget::cancelUpload, this, &RoomWidgetBase::slotCancelUpload);

    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    mMessageListView->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::TopEdge | Qt::BottomEdge}));
#endif
    mMainLayout->addWidget(mMessageListView, 1);

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

    mMainLayout->addWidget(mRoomReplyThreadWidget);
    mMainLayout->addWidget(mRoomQuoteMessageWidget);

    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mMainLayout->addWidget(mStackedWidget);

    mMessageLineWidget->setObjectName(QStringLiteral("mMessageLineWidget"));
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    mMessageLineWidget->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::TopEdge}));
#endif
    mStackedWidget->addWidget(mMessageLineWidget);

    mReadOnlyLineEditWidget->setObjectName(QStringLiteral("mReadOnlyLineEditWidget"));
    mStackedWidget->addWidget(mReadOnlyLineEditWidget);

    mStackedWidget->setCurrentWidget(mMessageLineWidget);

    connect(mMessageLineWidget, &MessageLineWidget::keyPressed, this, &RoomWidgetBase::keyPressedInLineEdit);
    if (mode == MessageListView::Mode::Editing) {
        connect(mMessageLineWidget, &MessageLineWidget::threadMessageIdChanged, this, &RoomWidgetBase::slotShowThreadMessage);
    }
    connect(mMessageLineWidget, &MessageLineWidget::quoteMessageChanged, this, &RoomWidgetBase::slotShowQuoteMessage);
    connect(mMessageLineWidget, &MessageLineWidget::createUploadJob, this, &RoomWidgetBase::slotSendFile);

    connect(mMessageListView, &MessageListView::editMessageRequested, mMessageLineWidget, &MessageLineWidget::setEditMessage);
    connect(mMessageListView, &MessageListView::quoteMessageRequested, mMessageLineWidget, &MessageLineWidget::setQuoteMessage);
    connect(mMessageListView, &MessageListView::createNewDiscussion, this, &RoomWidgetBase::createNewDiscussion);
    connect(mMessageListView, &MessageListView::createPrivateConversation, this, &RoomWidgetBase::slotCreatePrivateDiscussion);
    connect(mMessageListView, &MessageListView::loadHistoryRequested, this, &RoomWidgetBase::loadHistory);
    connect(mMessageListView, &MessageListView::replyInThreadRequested, mMessageLineWidget, [this](const QString &messageId, const QString &text) {
        mMessageLineWidget->setThreadMessageId(messageId, text, false);
    });

    connect(mMessageListView, &MessageListView::textToSpeech, this, &RoomWidgetBase::textToSpeech);

    connect(mMessageListView, &MessageListView::errorMessage, this, &RoomWidgetBase::errorMessage);
    connect(mMessageListView, &MessageListView::successMessage, this, &RoomWidgetBase::successMessage);
    setAcceptDrops(true);
}

RoomWidgetBase::~RoomWidgetBase() = default;

void RoomWidgetBase::slotSendFile(const RocketChatRestApi::UploadFileJob::UploadFileInfo &uploadFileInfo)
{
    const int identifier = mCurrentRocketChatAccount->uploadFileManager()->addUpload(uploadFileInfo);
    if (identifier != -1) {
        mUploadFileProgressStatusListWidget->addProgressStatusWidget(identifier);
    }
}

void RoomWidgetBase::slotShowThreadMessage(const QString &threadMessageId, const QString &text)
{
    mRoomReplyThreadWidget->setMessageText(text);
    mRoomReplyThreadWidget->setVisible(!threadMessageId.isEmpty());
}

void RoomWidgetBase::slotCancelUpload(int identifier)
{
    mCurrentRocketChatAccount->uploadFileManager()->cancelJob(identifier);
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
        if (mRoomReplyThreadWidget->isVisible()) {
            Q_EMIT mRoomReplyThreadWidget->cancelReplyingInThread();
        } else if (mRoomQuoteMessageWidget->isVisible()) {
            Q_EMIT mRoomQuoteMessageWidget->cancelQuoteMessage();
        } else {
            slotClearNotification();
        }
        ev->accept();
    } else if (ev->matches(QKeySequence::Copy) && mMessageLineWidget->messageTextEdit()->textCursor().selectedText().isEmpty()) {
        mMessageListView->copyMessageToClipboard();
        ev->accept();
    } else {
        mMessageListView->handleKeyPressEvent(ev);
    }
}

void RoomWidgetBase::slotUploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info, int jobIdentifier, const QString &accountName)
{
    mUploadFileProgressStatusListWidget->uploadProgress(info, jobIdentifier, accountName);
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
        disconnect(mCurrentRocketChatAccount->uploadFileManager(), &UploadFileManager::uploadProgress, this, &RoomWidgetBase::slotUploadProgress);
        disconnect(mCurrentRocketChatAccount,
                   &RocketChatAccount::ownUserPreferencesChanged,
                   mMessageLineWidget,
                   &MessageLineWidget::slotOwnUserPreferencesChanged);
        // hide it when we switch account.
        mUploadFileProgressStatusListWidget->setVisible(false);
    }

    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged, mMessageLineWidget, &MessageLineWidget::slotPublicSettingChanged);
    connect(mCurrentRocketChatAccount->uploadFileManager(), &UploadFileManager::uploadProgress, this, &RoomWidgetBase::slotUploadProgress);
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
        auto room = mCurrentRocketChatAccount->room(mRoomId);
        if (room && ((room->unread() > 0) || room->alert())) {
            mCurrentRocketChatAccount->markRoomAsRead(mRoomId);
        }
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

#include "moc_roomwidgetbase.cpp"
