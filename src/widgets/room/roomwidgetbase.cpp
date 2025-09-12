/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomwidgetbase.h"
using namespace Qt::Literals::StringLiterals;

#include "commandpreviewwidget.h"
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
    , mCommandPreviewWidget(new CommandPreviewWidget(this))
    , mReadOnlyLineEditWidget(new ReadOnlyLineEditWidget(this))
    , mMainLayout(new QVBoxLayout(this))
{
    mMainLayout->setObjectName(u"mainLayout"_s);
    mMainLayout->setContentsMargins({});
    mMainLayout->setSpacing(0);

    mUploadFileProgressStatusListWidget->setObjectName(u"mUploadFileProgressStatusListWidget"_s);
    mUploadFileProgressStatusListWidget->setVisible(false);
    mMainLayout->addWidget(mUploadFileProgressStatusListWidget);
    connect(mUploadFileProgressStatusListWidget, &UploadFileProgressStatusListWidget::cancelUpload, this, &RoomWidgetBase::slotCancelUpload);

    mMessageListView->setObjectName(u"mMessageListView"_s);
    mMessageListView->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::TopEdge | Qt::BottomEdge}));
    mMainLayout->addWidget(mMessageListView, 1);

    mRoomReplyThreadWidget->setObjectName(u"mRoomReplyThreadWidget"_s);
    mRoomReplyThreadWidget->setVisible(false);

    connect(mRoomReplyThreadWidget, &RoomReplyThreadWidget::cancelReplyingInThread, this, [this] {
        mMessageLineWidget->setThreadMessageId({});
    });

    mRoomQuoteMessageWidget->setObjectName(u"mRoomQuoteMessageWidget"_s);
    mRoomQuoteMessageWidget->setVisible(false);

    connect(mRoomQuoteMessageWidget, &RoomQuoteMessageWidget::cancelQuoteMessage, this, [this] {
        mMessageLineWidget->setQuoteMessage({}, QString());
    });

    mMainLayout->addWidget(mRoomReplyThreadWidget);
    mMainLayout->addWidget(mRoomQuoteMessageWidget);

    mCommandPreviewWidget->setObjectName(u"mCommandPreviewWidget"_s);
    mCommandPreviewWidget->setVisible(false);
    mMainLayout->addWidget(mCommandPreviewWidget);

    connect(mMessageLineWidget, &MessageLineWidget::showCommandPreview, mCommandPreviewWidget, &CommandPreviewWidget::setPreviewCommandInfo);
    connect(mCommandPreviewWidget, &CommandPreviewWidget::sendPreviewCommandInfo, mMessageLineWidget, &MessageLineWidget::setSendPreviewCommandInfo);

    mStackedWidget->setObjectName(u"mStackedWidget"_s);
    mMainLayout->addWidget(mStackedWidget);

    mMessageLineWidget->setObjectName(u"mMessageLineWidget"_s);
    mMessageLineWidget->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::TopEdge}));
    mStackedWidget->addWidget(mMessageLineWidget);

    mReadOnlyLineEditWidget->setObjectName(u"mReadOnlyLineEditWidget"_s);
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
    connect(mMessageListView, &MessageListView::replyInThreadRequested, mMessageLineWidget, [this](const QByteArray &messageId, const QString &text) {
        mMessageLineWidget->setThreadMessageId(messageId, text, false);
    });

    connect(mMessageListView, &MessageListView::textToSpeech, this, &RoomWidgetBase::textToSpeech);

    connect(mMessageListView, &MessageListView::errorMessage, this, &RoomWidgetBase::errorMessage);
    connect(mMessageListView, &MessageListView::successMessage, this, &RoomWidgetBase::successMessage);
    connect(mMessageListView, &MessageListView::uiInteractionRequested, this, &RoomWidgetBase::uiInteractionRequested);
    connect(mMessageLineWidget, &MessageLineWidget::uiInteractionRequested, this, &RoomWidgetBase::uiInteractionRequested);
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

void RoomWidgetBase::slotShowThreadMessage(const QByteArray &threadMessageId, const QString &text)
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

void RoomWidgetBase::slotCreateNewDiscussion(const QByteArray &messageId, const QString &originalMessage, const QString &channelName)
{
    CreateNewDiscussionDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setDiscussionName(originalMessage);
    dlg.setChannelInfo(channelName, mRoomId);
    dlg.setMessageId(messageId);
    dlg.exec();
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
        } else if (mCommandPreviewWidget->isVisible()) {
            mCommandPreviewWidget->hidePreview();
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

QByteArray RoomWidgetBase::roomId() const
{
    return mRoomId;
}

void RoomWidgetBase::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

void RoomWidgetBase::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged, mMessageLineWidget, &MessageLineWidget::slotPublicSettingChanged);
        disconnect(mCurrentRocketChatAccount->uploadFileManager(), &UploadFileManager::uploadProgress, this, &RoomWidgetBase::slotUploadProgress);
        disconnect(mCurrentRocketChatAccount,
                   &RocketChatAccount::ownUserUiPreferencesChanged,
                   mMessageLineWidget,
                   &MessageLineWidget::slotOwnUserPreferencesChanged);
        // hide it when we switch account.
        mUploadFileProgressStatusListWidget->setVisible(false);
    }

    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged, mMessageLineWidget, &MessageLineWidget::slotPublicSettingChanged);
    connect(mCurrentRocketChatAccount->uploadFileManager(), &UploadFileManager::uploadProgress, this, &RoomWidgetBase::slotUploadProgress);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::ownUserUiPreferencesChanged, mMessageLineWidget, &MessageLineWidget::slotOwnUserPreferencesChanged);
    mMessageListView->setCurrentRocketChatAccount(account);
    mMessageLineWidget->setCurrentRocketChatAccount(account, false);
    mCommandPreviewWidget->setCurrentRocketChatAccount(account);
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
