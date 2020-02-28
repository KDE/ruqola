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
#include "messagetextedit.h"
#include "ruqolawidgets_debug.h"
#include "usersinroomflowwidget.h"
#include "dialogs/createnewdiscussiondialog.h"
#include "dialogs/searchmessagedialog.h"
#include "dialogs/configurenotificationdialog.h"
#include "dialogs/showattachmentdialog.h"
#include "dialogs/showdiscussionsdialog.h"
#include "dialogs/showmentionsmessagesdialog.h"
#include "dialogs/showpinnedmessagesdialog.h"
#include "dialogs/showsnipperedmessagesdialog.h"
#include "dialogs/showstarredmessagesdialog.h"
#include "dialogs/showthreadsdialog.h"

#include "threadwidget/threadmessagedialog.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QStackedWidget>
#include <QMimeData>
#include <QPointer>
#include <QScrollBar>
#include <QTemporaryFile>
#include <QDir>
#include <QImageWriter>

RoomWidget::RoomWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mRoomHeaderWidget = new RoomHeaderWidget(this);
    mRoomHeaderWidget->setObjectName(QStringLiteral("mRoomHeaderWidget"));
    mainLayout->addWidget(mRoomHeaderWidget);

    mUsersInRoomFlowWidget = new UsersInRoomFlowWidget(this);
    mUsersInRoomFlowWidget->setObjectName(QStringLiteral("mUsersInRoomFlowWidget"));
    mainLayout->addWidget(mUsersInRoomFlowWidget);
    mUsersInRoomFlowWidget->setVisible(false);

    mMessageListView = new MessageListView(this);
    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    mainLayout->addWidget(mMessageListView, 1);

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

    connect(mMessageLineWidget, &MessageLineWidget::keyPressed, this, &RoomWidget::keyPressedInLineEdit);

    connect(mRoomHeaderWidget, &RoomHeaderWidget::favoriteChanged, this, &RoomWidget::slotChangeFavorite);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::encryptedChanged, this, &RoomWidget::slotEncryptedChanged);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::goBackToRoom, this, &RoomWidget::slotGoBackToRoom);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::listOfUsersChanged, this, &RoomWidget::slotShowListOfUsersInRoom);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::searchMessageRequested, this, &RoomWidget::slotSearchMessages);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::actionRequested, this, &RoomWidget::slotActionRequested);

    connect(mMessageListView, &MessageListView::editMessageRequested, mMessageLineWidget, &MessageLineWidget::setEditMessage);
    connect(mMessageListView, &MessageListView::createNewDiscussion, this, &RoomWidget::slotCreateNewDiscussion);
    connect(mMessageListView, &MessageListView::loadHistoryRequested, this, &RoomWidget::slotLoadHistory);

    setAcceptDrops(true);
}

RoomWidget::~RoomWidget()
{
    delete mRoomWrapper;
}

void RoomWidget::slotLoadHistory()
{
    mCurrentRocketChatAccount->loadHistory(mRoomId, mRoomType);
}

void RoomWidget::slotActionRequested(RoomHeaderWidget::ChannelActionType type)
{
    switch (type) {
    case RoomHeaderWidget::ShowMentions:
        slotShowMentions();
        break;
    case RoomHeaderWidget::ShowPinned:
        slotPinnedMessages();
        break;
    case RoomHeaderWidget::ShowStarred:
        slotStarredMessages();
        break;
    case RoomHeaderWidget::ShowSnippered:
        slotSnipperedMessages();
        break;
    case RoomHeaderWidget::ShowDiscussions:
        slotShowDiscussions();
        break;
    case RoomHeaderWidget::ShowThreads:
        slotShowThreads();
        break;
    case RoomHeaderWidget::ShowAttachment:
        slotShowFileAttachments();
        break;
    case RoomHeaderWidget::Notification:
        slotConfigureNotification();
        break;
    }
}

void RoomWidget::slotConfigureNotification()
{
    QPointer<ConfigureNotificationDialog> dlg = new ConfigureNotificationDialog(this);
    dlg->setRoomWrapper(mRoomWrapper);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotStarredMessages()
{
    QPointer<ShowStarredMessagesDialog> dlg = new ShowStarredMessagesDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::StarredMessages);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotPinnedMessages()
{
    QPointer<ShowPinnedMessagesDialog> dlg = new ShowPinnedMessagesDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::PinnedMessages);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowMentions()
{
    QPointer<ShowMentionsMessagesDialog> dlg = new ShowMentionsMessagesDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::MentionsMessages);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotSnipperedMessages()
{
    QPointer<ShowSnipperedMessagesDialog> dlg = new ShowSnipperedMessagesDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::SnipperedMessages);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowThreads()
{
    QPointer<ShowThreadsDialog> dlg = new ShowThreadsDialog(this);
    dlg->setModel(mCurrentRocketChatAccount->threadsFilterProxyModel());
    dlg->setRoomId(mRoomId);
    mCurrentRocketChatAccount->threadsInRoom(mRoomId);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowDiscussions()
{
    QPointer<ShowDiscussionsDialog> dlg = new ShowDiscussionsDialog(this);
    dlg->setModel(mCurrentRocketChatAccount->discussionsFilterProxyModel());
    dlg->setRoomId(mRoomId);
    mCurrentRocketChatAccount->discussionsInRoom(mRoomId);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowFileAttachments()
{
    QPointer<ShowAttachmentDialog> dlg = new ShowAttachmentDialog(this);
    mCurrentRocketChatAccount->roomFiles(mRoomId, mRoomType);
    dlg->setModel(mCurrentRocketChatAccount->filesForRoomFilterProxyModel());
    dlg->setRoomId(mRoomId);
    dlg->setRoomType(mRoomType);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotSearchMessages()
{
    QPointer<SearchMessageDialog> dlg = new SearchMessageDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage)
{
    QPointer<CreateNewDiscussionDialog> dlg = new CreateNewDiscussionDialog(this);
    dlg->setDiscussionName(originalMessage);
    dlg->setChannelName(mRoomHeaderWidget->roomName());
    if (dlg->exec()) {
        const CreateNewDiscussionDialog::NewDiscussionInfo info = dlg->newDiscussionInfo();
        mCurrentRocketChatAccount->createDiscussion(mRoomId, info.discussionName, info.message, messageId, info.users);
    }
    delete dlg;
}

void RoomWidget::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        event->accept();
    }
}

void RoomWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        mMessageLineWidget->handleMimeData(mimeData);
    }
}

void RoomWidget::setChannelSelected(const QString &roomId, const QString &roomType)
{
    if (mMessageLineWidget->text().isEmpty()) {
        auto *vbar = mMessageListView->verticalScrollBar();
        if (vbar->value() != vbar->maximum()) {
            AccountRoomSettings::PendingTypedInfo info;
            info.scrollbarPosition = mMessageListView->verticalScrollBar()->value();
            mCurrentRocketChatAccount->accountRoomSettings()->add(mRoomId, info);
        } else {
            mCurrentRocketChatAccount->accountRoomSettings()->remove(mRoomId);
        }
    } else {
        AccountRoomSettings::PendingTypedInfo info;
        info.text = mMessageLineWidget->text();
        info.messageIdBeingEdited = mMessageLineWidget->messageIdBeingEdited();
        info.scrollbarPosition = mMessageListView->verticalScrollBar()->value();
        mCurrentRocketChatAccount->accountRoomSettings()->add(mRoomId, info);
    }

    setRoomId(roomId);
    setRoomType(roomType);
    const AccountRoomSettings::PendingTypedInfo currentPendingInfo = mCurrentRocketChatAccount->accountRoomSettings()->value(roomId);
    if (currentPendingInfo.isValid()) {
        mMessageLineWidget->setText(currentPendingInfo.text);
        mMessageLineWidget->setMessageIdBeingEdited(currentPendingInfo.messageIdBeingEdited);
        if (currentPendingInfo.scrollbarPosition != -1) {
            mMessageListView->verticalScrollBar()->setValue(currentPendingInfo.scrollbarPosition);
        }
    } else {
        mMessageLineWidget->setText(QString());
    }
    mMessageLineWidget->setMode(mMessageLineWidget->messageIdBeingEdited().isEmpty() ? MessageLineWidget::EditingMode::NewMessage : MessageLineWidget::EditingMode::EditMessage);

    mMessageLineWidget->setFocus();
}

void RoomWidget::updateRoomHeader()
{
    if (mRoomWrapper) {
        mRoomHeaderWidget->setRoomName(mRoomWrapper->displayRoomName());
        mRoomHeaderWidget->setRoomAnnouncement(mRoomWrapper->announcement());
        mRoomHeaderWidget->setRoomTopic(mRoomWrapper->topic());
        mRoomHeaderWidget->setFavoriteStatus(mRoomWrapper->favorite());
        mRoomHeaderWidget->setEncypted(mRoomWrapper->encrypted());
        mRoomHeaderWidget->setIsDiscussion(mRoomWrapper->isDiscussionRoom());
        //TODO Description ?

        if (mRoomWrapper->roomMessageInfo().isEmpty()) {
            mStackedWidget->setCurrentWidget(mMessageLineWidget);
        } else {
            mStackedWidget->setCurrentWidget(mReadOnlyLineEditWidget);
            mReadOnlyLineEditWidget->setMessage(mRoomWrapper->roomMessageInfo());
        }
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

void RoomWidget::slotShowListOfUsersInRoom(bool checked)
{
    mUsersInRoomFlowWidget->setVisible(checked);
}

void RoomWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
    delete mRoomWrapper;
    mRoomWrapper = mCurrentRocketChatAccount->roomWrapper(mRoomId);
    connectRoomWrapper();
    mMessageLineWidget->setRoomId(roomId);
    mMessageListView->setChannelSelected(roomId);
    mUsersInRoomFlowWidget->setRoomWrapper(mRoomWrapper);
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
            mRoomHeaderWidget->setRoomName(mRoomWrapper->displayRoomName());
        });
        connect(mRoomWrapper, &RoomWrapper::favoriteChanged, this, [this]() {
            mRoomHeaderWidget->setFavoriteStatus(mRoomWrapper->favorite());
        });
        connect(mRoomWrapper, &RoomWrapper::encryptedChanged, this, [this]() {
            mRoomHeaderWidget->setEncypted(mRoomWrapper->encrypted());
        });
    }
    updateRoomHeader();
}

void RoomWidget::slotClearNotification()
{
    mCurrentRocketChatAccount->clearUnreadMessages(mRoomId);
}

void RoomWidget::slotEncryptedChanged(bool b)
{
    qCWarning(RUQOLAWIDGETS_LOG) << "change encrypted not supported yet";
    //TODO mCurrentRocketChatAccount->slot
}

void RoomWidget::slotChangeFavorite(bool b)
{
    mCurrentRocketChatAccount->changeFavorite(mRoomId, b);
}

void RoomWidget::keyPressedInLineEdit(QKeyEvent *ev)
{
    const int key = ev->key();
    if (key == Qt::Key_Escape) {
        slotClearNotification();
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
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::openThreadRequested,
                   this, &RoomWidget::slotOpenThreadRequested);
    }

    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openThreadRequested,
            this, &RoomWidget::slotOpenThreadRequested);
    mMessageLineWidget->setCurrentRocketChatAccount(account);
    mRoomHeaderWidget->setCurrentRocketChatAccount(account);
    mRoomId.clear(); //Clear it otherwise if we switch between two account with same roomId (as "GENERAL") we will see incorrect room.
}

void RoomWidget::slotGoBackToRoom()
{
    Q_EMIT selectChannelRequested(mRoomWrapper->parentRid());
}

void RoomWidget::slotOpenThreadRequested(const QString &threadMessageId)
{
    QPointer<ThreadMessageDialog> dlg = new ThreadMessageDialog(this);
    dlg->setThreadMessageId(threadMessageId);
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoomId(mRoomId);
    dlg->exec();
    delete dlg;
}
