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
#include "messagelistview.h"
#include "messagelinewidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "room.h"
#include "readonlylineeditwidget.h"
#include "messagetextedit.h"
#include "roomutil.h"
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
#include "dialogs/autotranslateconfiguredialog.h"
#include "dialogs/channelinfodialog.h"
#include "dialogs/directchannelinfodialog.h"
#include "dialogs/inviteusersdialog.h"
#include "dialogs/addusersinroomdialog.h"
#include "prunemessages/prunemessagesdialog.h"
#include "exportmessages/exportmessagesdialog.h"

#include "threadwidget/threadmessagedialog.h"
#include "roomcounterinfowidget.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QStackedWidget>
#include <QMimeData>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>

RoomWidget::RoomWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mRoomHeaderWidget = new RoomHeaderWidget(this);
    mRoomHeaderWidget->setObjectName(QStringLiteral("mRoomHeaderWidget"));
    mainLayout->addWidget(mRoomHeaderWidget);

    mUsersInRoomFlowWidget = new UsersInRoomFlowWidget(this);
    mUsersInRoomFlowWidget->setObjectName(QStringLiteral("mUsersInRoomFlowWidget"));
    mainLayout->addWidget(mUsersInRoomFlowWidget);
    mUsersInRoomFlowWidget->setVisible(false);

    mRoomCounterInfoWidget = new RoomCounterInfoWidget(this);
    mRoomCounterInfoWidget->setObjectName(QStringLiteral("mRoomCounterInfoWidget"));
    mainLayout->addWidget(mRoomCounterInfoWidget);

    mMessageListView = new MessageListView(MessageListView::Mode::Editing, this);
    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    mainLayout->addWidget(mMessageListView, 1);

    mMessageThreadWidget = new QWidget(this);
    mMessageThreadWidget->setObjectName(QStringLiteral("mMessageThreadWidget"));
    mMessageThreadWidget->setVisible(false);
    auto messageThreadLayout = new QHBoxLayout(mMessageThreadWidget);
    mMessageThreadWidget->setLayout(messageThreadLayout);
    mMessageThreadLabel = new QLabel(i18n("Replying in a thread"));
    messageThreadLayout->addWidget(mMessageThreadLabel);
    mMessageThreadButton = new QPushButton(mMessageThreadWidget);
    mMessageThreadButton->setText(i18n("Cancel"));
    mMessageThreadButton->setIcon(QIcon::fromTheme(QStringLiteral("dialog-close")));
    connect(mMessageThreadButton, &QPushButton::clicked, this, [this] {
        mMessageLineWidget->setThreadMessageId({});
    });
    messageThreadLayout->addWidget(mMessageThreadButton);
    messageThreadLayout->addStretch();
    mainLayout->addWidget(mMessageThreadWidget);

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
    connect(mMessageLineWidget, &MessageLineWidget::threadMessageIdChanged, this, &RoomWidget::slotShowThreadMessage);

    connect(mRoomHeaderWidget, &RoomHeaderWidget::favoriteChanged, this, &RoomWidget::slotChangeFavorite);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::encryptedChanged, this, &RoomWidget::slotEncryptedChanged);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::goBackToRoom, this, &RoomWidget::slotGoBackToRoom);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::listOfUsersChanged, this, &RoomWidget::slotShowListOfUsersInRoom);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::searchMessageRequested, this, &RoomWidget::slotSearchMessages);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::actionRequested, this, &RoomWidget::slotActionRequested);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::channelInfoRequested, this, &RoomWidget::slotChannelInfoRequested);

    connect(mMessageListView, &MessageListView::editMessageRequested, mMessageLineWidget, &MessageLineWidget::setEditMessage);
    connect(mMessageListView, &MessageListView::createNewDiscussion, this, &RoomWidget::slotCreateNewDiscussion);
    connect(mMessageListView, &MessageListView::createPrivateConversation, this, &RoomWidget::slotCreatePrivateDiscussion);
    connect(mMessageListView, &MessageListView::loadHistoryRequested, this, &RoomWidget::slotLoadHistory);
    connect(mMessageListView, &MessageListView::replyInThreadRequested, mMessageLineWidget, [this](const QString &messageId) {
        mMessageLineWidget->setThreadMessageId(messageId);
    });

    setAcceptDrops(true);
}

RoomWidget::~RoomWidget()
{
    delete mRoom;
}

void RoomWidget::slotLoadHistory()
{
    mCurrentRocketChatAccount->loadHistory(mRoomId, mRoomType);
}

void RoomWidget::slotChannelInfoRequested()
{
    if (mRoomType == QLatin1String("d")) {
        QPointer<DirectChannelInfoDialog> dlg = new DirectChannelInfoDialog(this);
        dlg->setUserName(mRoom->name());
        dlg->exec();
        delete dlg;
    } else {
        QPointer<ChannelInfoDialog> dlg = new ChannelInfoDialog(this);
        dlg->setRoom(mRoom);
        dlg->exec();
        delete dlg;
    }
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
    case RoomHeaderWidget::AutoTranslate:
        slotConfigureAutoTranslate();
        break;
    case RoomHeaderWidget::InviteUsers:
        slotInviteUsers();
        break;
    case RoomHeaderWidget::AddUsersInRoom:
        slotAddUsersInRoom();
        break;
    case RoomHeaderWidget::VideoChat:
        slotVideoChat();
        break;
    case RoomHeaderWidget::PruneMessages:
        slotPruneMessages();
        break;
    case RoomHeaderWidget::ExportMessages:
        slotExportMessages();
        break;
    }
}

void RoomWidget::slotPruneMessages()
{
    QPointer<PruneMessagesDialog> dlg = new PruneMessagesDialog(this);
    if (dlg->exec()) {
        RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo info = dlg->cleanHistoryInfo();
        info.roomId = mRoomId;
        if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Do you want really remove history?"), i18n("Remove History"))) {
            mCurrentRocketChatAccount->cleanChannelHistory(info);
        }
    }
    delete dlg;
}

void RoomWidget::slotExportMessages()
{
    QPointer<ExportMessagesDialog> dlg = new ExportMessagesDialog(this);
    if (dlg->exec()) {
        RocketChatRestApi::RoomsExportJob::RoomsExportInfo info = dlg->roomExportInfo();
        info.roomId = mRoomId;
        mCurrentRocketChatAccount->exportMessages(info);
    }
    delete dlg;
}

void RoomWidget::slotVideoChat()
{
    mCurrentRocketChatAccount->createJitsiConfCall(mRoomId);
}

void RoomWidget::slotAddUsersInRoom()
{
    QPointer<AddUsersInRoomDialog> dlg = new AddUsersInRoomDialog(this);
    if (dlg->exec()) {
        const QStringList users = dlg->users();
        for (const QString &user : users) {
            mCurrentRocketChatAccount->addUserToRoom(user, mRoomId, mRoomType);
        }
    }
    delete dlg;
}

void RoomWidget::slotInviteUsers()
{
    QPointer<InviteUsersDialog> dlg = new InviteUsersDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->generateLink();
    dlg->exec();
    delete dlg;
}

void RoomWidget::updateListView()
{
    mMessageListView->viewport()->update();
}

void RoomWidget::slotConfigureAutoTranslate()
{
    QPointer<AutoTranslateConfigureDialog> dlg = new AutoTranslateConfigureDialog(this);
    dlg->setRoom(mRoom);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotConfigureNotification()
{
    QPointer<ConfigureNotificationDialog> dlg = new ConfigureNotificationDialog(this);
    dlg->setRoom(mRoom);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotStarredMessages()
{
    QPointer<ShowStarredMessagesDialog> dlg = new ShowStarredMessagesDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::StarredMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mMessageListView, &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotPinnedMessages()
{
    QPointer<ShowPinnedMessagesDialog> dlg = new ShowPinnedMessagesDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoom(mRoom);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::PinnedMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mMessageListView, &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowMentions()
{
    QPointer<ShowMentionsMessagesDialog> dlg = new ShowMentionsMessagesDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::MentionsMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mMessageListView, &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotSnipperedMessages()
{
    QPointer<ShowSnipperedMessagesDialog> dlg = new ShowSnipperedMessagesDialog(this);
    dlg->setRoomId(mRoomId);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::SnipperedMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mMessageListView, &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowThreads()
{
    QPointer<ShowThreadsDialog> dlg = new ShowThreadsDialog(this);
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setRoomId(mRoomId);
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomId, ListMessagesModel::ThreadsMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mMessageListView, &MessageListView::goToMessage);
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
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoomId(mRoomId);
    dlg->setRoom(mRoom);
    dlg->setModel(mCurrentRocketChatAccount->searchMessageFilterProxyModel());
    connect(dlg, &SearchMessageDialog::goToMessageRequested, mMessageListView, &MessageListView::goToMessage);
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

void RoomWidget::slotCreatePrivateDiscussion(const QString &userName)
{
    Q_EMIT mCurrentRocketChatAccount->openLinkRequested(RoomUtil::generateUserLink(userName));
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

void RoomWidget::storeRoomSettings()
{
    if (mCurrentRocketChatAccount) {
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
    }
}

void RoomWidget::setChannelSelected(const QString &roomId, const QString &roomType)
{
    storeRoomSettings();
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

void RoomWidget::slotUpdateRoomCounterInfoWidget()
{
    if (mRoom) {
        mRoomCounterInfoWidget->setChannelCounterInfo(mRoom->channelCounterInfo());
    }
}

void RoomWidget::updateRoomHeader()
{
    if (mRoom) {
        mRoomHeaderWidget->setRoomName(mRoom->displayRoomName());
        mRoomHeaderWidget->setRoomAnnouncement(mRoom->announcement());
        mRoomHeaderWidget->setRoomTopic(mRoom->displayTopic());
        mRoomHeaderWidget->setFavoriteStatus(mRoom->favorite());
        mRoomHeaderWidget->setEncypted(mRoom->encrypted() && mRoom->hasPermission(QStringLiteral("edit-room")));
        mRoomHeaderWidget->setIsDiscussion(mRoom->isDiscussionRoom());
        //TODO Description ?

        if (mRoom->roomMessageInfo().isEmpty()) {
            mStackedWidget->setCurrentWidget(mMessageLineWidget);
        } else {
            mStackedWidget->setCurrentWidget(mReadOnlyLineEditWidget);
            mReadOnlyLineEditWidget->setMessage(mRoom->roomMessageInfo());
        }
        if (mRoom->channelCounterInfo().isValid() && mRoom->channelCounterInfo().unreadMessages() > 0) {
            mRoomCounterInfoWidget->animatedShow();
        } else {
            mRoomCounterInfoWidget->animatedHide();
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

Room *RoomWidget::room() const
{
    return mRoom;
}

void RoomWidget::slotShowListOfUsersInRoom(bool checked)
{
    mUsersInRoomFlowWidget->setVisible(checked);
}

void RoomWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
    mRoom = mCurrentRocketChatAccount->room(mRoomId);
    if (mRoom) {
        connectRoom();
        mMessageLineWidget->setRoomId(roomId);
        mMessageListView->setChannelSelected(mRoom);
        mUsersInRoomFlowWidget->setRoom(mRoom);
        mRoomHeaderWidget->setRoom(mRoom);
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << " Impossible to find room " << roomId;
    }
}

void RoomWidget::connectRoom()
{
    if (mRoom) {
        connect(mRoom, &Room::announcementChanged, this, [this]() {
            mRoomHeaderWidget->setRoomAnnouncement(mRoom->announcement());
        });
        connect(mRoom, &Room::topicChanged, this, [this]() {
            mRoomHeaderWidget->setRoomTopic(mRoom->displayTopic());
        });
        connect(mRoom, &Room::nameChanged, this, [this]() {
            mRoomHeaderWidget->setRoomName(mRoom->displayRoomName());
        });
        connect(mRoom, &Room::favoriteChanged, this, [this]() {
            mRoomHeaderWidget->setFavoriteStatus(mRoom->favorite());
        });
        connect(mRoom, &Room::encryptedChanged, this, [this]() {
            mRoomHeaderWidget->setEncypted(mRoom->encrypted());
        });
        connect(mRoom, &Room::autoTranslateLanguageChanged, this, &RoomWidget::updateListView);
        connect(mRoom, &Room::autoTranslateChanged, this, &RoomWidget::updateListView);
        connect(mRoom, &Room::ignoredUsersChanged, this, &RoomWidget::updateListView);
        connect(mRoom, &Room::channelCounterInfoChanged, this, &RoomWidget::slotUpdateRoomCounterInfoWidget);
    }
    slotUpdateRoomCounterInfoWidget();
    updateRoomHeader();
}

void RoomWidget::slotClearNotification()
{
    mCurrentRocketChatAccount->markRoomAsRead(mRoomId);
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
    } else if (ev->matches(QKeySequence::Copy) && mMessageLineWidget->messageTextEdit()->textCursor().selectedText().isEmpty()) {
        mMessageListView->copyMessageToClipboard();
        ev->accept();
    } else {
        mMessageListView->handleKeyPressEvent(ev);
    }
}

void RoomWidget::slotShowThreadMessage(const QString &threadMessageId)
{
    mMessageThreadWidget->setVisible(!threadMessageId.isEmpty());
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
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged,
                   mMessageLineWidget, &MessageLineWidget::slotPublicSettingChanged);
    }

    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openThreadRequested,
            this, &RoomWidget::slotOpenThreadRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged,
            mMessageLineWidget, &MessageLineWidget::slotPublicSettingChanged);
    mMessageListView->setCurrentRocketChatAccount(account);
    mMessageLineWidget->setCurrentRocketChatAccount(account, false);
    mRoomHeaderWidget->setCurrentRocketChatAccount(account);
    mRoomId.clear(); //Clear it otherwise if we switch between two account with same roomId (as "GENERAL") we will see incorrect room.
}

void RoomWidget::slotGoBackToRoom()
{
    Q_EMIT selectChannelRequested(mRoom->parentRid());
}

void RoomWidget::slotOpenThreadRequested(const QString &threadMessageId, const QString &threadMessagePreview)
{
    QPointer<ThreadMessageDialog> dlg = new ThreadMessageDialog(this);
    dlg->setThreadMessageId(threadMessageId);
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setThreadPreview(threadMessagePreview);
    dlg->setRoom(mRoom);
    dlg->exec();
    delete dlg;
}
