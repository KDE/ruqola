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

#include <QApplication>
#include <QClipboard>
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

    connect(mMessageLineWidget, &MessageLineWidget::sendMessage, this, &RoomWidget::slotSendMessage);
    connect(mMessageLineWidget, &MessageLineWidget::sendFile, this, &RoomWidget::slotSendFile);
    connect(mMessageLineWidget, &MessageLineWidget::textEditing, this, &RoomWidget::slotTextEditing);
    connect(mMessageLineWidget->messageTextEdit(), &MessageTextEdit::keyPressed, this, &RoomWidget::keyPressedInLineEdit);

    connect(mRoomHeaderWidget, &RoomHeaderWidget::favoriteChanged, this, &RoomWidget::slotChangeFavorite);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::encryptedChanged, this, &RoomWidget::slotEncryptedChanged);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::goBackToRoom, this, &RoomWidget::slotGoBackToRoom);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::listOfUsersChanged, this, &RoomWidget::slotShowListOfUsersInRoom);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::searchMessageRequested, this, &RoomWidget::slotSearchMessages);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::actionRequested, this, &RoomWidget::slotActionRequested);

    connect(mMessageListView, &MessageListView::editMessageRequested, this, &RoomWidget::slotEditMessage);
    connect(mMessageListView, &MessageListView::createNewDiscussion, this, &RoomWidget::slotCreateNewDiscussion);

    setAcceptDrops(true);
}

RoomWidget::~RoomWidget()
{
    delete mRoomWrapper;
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
    dlg->setModel(Ruqola::self()->rocketChatAccount()->listMessagesFilterProxyModel());
    Ruqola::self()->rocketChatAccount()->getListMessages(mRoomId, ListMessagesModel::MentionsMessages);
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

void RoomWidget::slotTextEditing(bool clearNotification)
{
    mCurrentRocketChatAccount->textEditing(mRoomId, clearNotification);
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
    mMessageLineWidget->setMode(MessageLineWidget::EditingMode::NewMessage);
}

void RoomWidget::slotEditMessage(const QString &messageId, const QString &text)
{
    mMessageIdBeingEdited = messageId;
    mMessageLineWidget->setMode(MessageLineWidget::EditingMode::EditMessage);
    mMessageLineWidget->setText(text);
    mMessageLineWidget->setFocus();
}

void RoomWidget::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        event->accept();
    }
}

bool RoomWidget::handleMimeData(const QMimeData *mimeData)
{
    auto uploadFile = [this](const QUrl &url) {
        QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
        dlg->setFileUrl(url);
        if (dlg->exec()) {
            const UploadFileDialog::UploadFileInfo uploadFileInfo = dlg->fileInfo();
            slotSendFile(uploadFileInfo);
        }
    };
    if (mimeData->hasUrls()) {
        const QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls) {
            if (url.isLocalFile()) {
                uploadFile(url);
            }
        }
        return true;
    } else if (mimeData->hasImage()) {
        QTemporaryFile tempFile(QDir::tempPath() + QLatin1String("/XXXXXX.png"));
        if (tempFile.open()) {
            QImage image = mimeData->imageData().value<QImage>();
            QImageWriter writer(&tempFile, "PNG");
            if (writer.write(image)) {
                const QUrl url = QUrl::fromLocalFile(tempFile.fileName());
                tempFile.close();
                uploadFile(url);
                return true;
            }
        }
    }
    return false;
}

void RoomWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        handleMimeData(mimeData);
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
        info.messageIdBeingEdited = mMessageIdBeingEdited;
        info.scrollbarPosition = mMessageListView->verticalScrollBar()->value();
        mCurrentRocketChatAccount->accountRoomSettings()->add(mRoomId, info);
    }

    setRoomId(roomId);
    setRoomType(roomType);
    const AccountRoomSettings::PendingTypedInfo currentPendingInfo = mCurrentRocketChatAccount->accountRoomSettings()->value(roomId);
    if (currentPendingInfo.isValid()) {
        mMessageLineWidget->setText(currentPendingInfo.text);
        mMessageIdBeingEdited = currentPendingInfo.messageIdBeingEdited;
        if (currentPendingInfo.scrollbarPosition != -1) {
            mMessageListView->verticalScrollBar()->setValue(currentPendingInfo.scrollbarPosition);
        }
    } else {
        mMessageLineWidget->setText(QString());
    }
    mMessageLineWidget->setMode(mMessageIdBeingEdited.isEmpty() ? MessageLineWidget::EditingMode::NewMessage : MessageLineWidget::EditingMode::EditMessage);

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
    if (ev->key() == Qt::Key_Escape) {
        if (!mMessageIdBeingEdited.isEmpty()) {
            mMessageIdBeingEdited.clear();
            mMessageLineWidget->setText(QString());
        } else {
            slotClearNotification();
        }
        ev->accept();
    } else if (ev->matches(QKeySequence::Paste)) {
        const QMimeData *mimeData = qApp->clipboard()->mimeData();
        if (handleMimeData(mimeData)) {
            ev->accept();
            return;
        }
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
    mRoomHeaderWidget->setCurrentRocketChatAccount(account);
    mRoomId.clear(); //Clear it otherwise if we switch between two account with same roomId (as "GENERAL") we will see incorrect room.
}

void RoomWidget::slotGoBackToRoom()
{
    Q_EMIT selectChannelRequested(mRoomWrapper->parentRid());
}
