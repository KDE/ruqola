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

#include "roomwidget.h"
#include "dialogs/addusersinroomdialog.h"
#include "dialogs/autotranslateconfiguredialog.h"
#include "dialogs/channelinfodialog.h"
#include "dialogs/configurenotificationdialog.h"
#include "dialogs/createnewdiscussiondialog.h"
#include "dialogs/directchannelinfodialog.h"
#include "dialogs/inviteusersdialog.h"
#include "dialogs/searchmessagedialog.h"
#include "dialogs/showattachmentdialog.h"
#include "dialogs/showdiscussionsdialog.h"
#include "dialogs/showmentionsmessagesdialog.h"
#include "dialogs/showpinnedmessagesdialog.h"
#include "dialogs/showsnipperedmessagesdialog.h"
#include "dialogs/showstarredmessagesdialog.h"
#include "dialogs/showthreadsdialog.h"
#include "exportmessages/exportmessagesdialog.h"
#include "messagelinewidget.h"
#include "messagelistview.h"
#include "messagetextedit.h"
#include "prunemessages/prunemessagesdialog.h"
#include "readonlylineeditwidget.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "rocketchatbackend.h"
#include "room.h"
#include "roomutil.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "usersinroomflowwidget.h"

#include "roomcounterinfowidget.h"
#include "roomquotemessagewidget.h"
#include "roomreplythreadwidget.h"
#include "roomwidgetbase.h"
#include "threadwidget/threadmessagedialog.h"
#include "uploadfileprogressstatuswidget.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QMimeData>
#include <QPushButton>
#include <QScrollBar>
#include <QVBoxLayout>

RoomWidget::RoomWidget(QWidget *parent)
    : QWidget(parent)
    , mRoomWidgetBase(new RoomWidgetBase(MessageListView::Mode::Editing, this))
    , mRoomHeaderWidget(new RoomHeaderWidget(this))
    , mUsersInRoomFlowWidget(new UsersInRoomFlowWidget(this))
    , mRoomCounterInfoWidget(new RoomCounterInfoWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mRoomHeaderWidget->setObjectName(QStringLiteral("mRoomHeaderWidget"));
    mainLayout->addWidget(mRoomHeaderWidget);

    mUsersInRoomFlowWidget->setObjectName(QStringLiteral("mUsersInRoomFlowWidget"));
    mainLayout->addWidget(mUsersInRoomFlowWidget);
    mUsersInRoomFlowWidget->setVisible(false);

    mRoomCounterInfoWidget->setObjectName(QStringLiteral("mRoomCounterInfoWidget"));
    mainLayout->addWidget(mRoomCounterInfoWidget);
    mainLayout->addWidget(mRoomWidgetBase);
    connect(mRoomCounterInfoWidget, &RoomCounterInfoWidget::markAsRead, this, &RoomWidget::slotClearNotification);
    connect(mRoomCounterInfoWidget, &RoomCounterInfoWidget::jumpToUnreadMessage, this, &RoomWidget::slotJumpToUnreadMessage);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::favoriteChanged, this, &RoomWidget::slotChangeFavorite);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::encryptedChanged, this, &RoomWidget::slotEncryptedChanged);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::goBackToRoom, this, &RoomWidget::slotGoBackToRoom);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::listOfUsersChanged, this, &RoomWidget::slotShowListOfUsersInRoom);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::searchMessageRequested, this, &RoomWidget::slotSearchMessages);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::actionRequested, this, &RoomWidget::slotActionRequested);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::channelInfoRequested, this, &RoomWidget::slotChannelInfoRequested);
    connect(mRoomWidgetBase, &RoomWidgetBase::loadHistory, this, &RoomWidget::slotLoadHistory);
    connect(mRoomWidgetBase, &RoomWidgetBase::createNewDiscussion, this, &RoomWidget::slotCreateNewDiscussion);
    setAcceptDrops(true);
}

RoomWidget::~RoomWidget()
{
    delete mRoom;
}

void RoomWidget::slotLoadHistory()
{
    mCurrentRocketChatAccount->loadHistory(mRoomWidgetBase->roomId(), mRoomType);
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
        if (dlg->exec()) {
            const RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo info = dlg->saveRoomSettingsInfo();
            if (info.isValid()) {
                auto saveRoomSettingsJob = new RocketChatRestApi::SaveRoomSettingsJob(this);
                saveRoomSettingsJob->setSaveRoomSettingsInfo(info);
                mCurrentRocketChatAccount->restApi()->initializeRestApiJob(saveRoomSettingsJob);
                if (!saveRoomSettingsJob->start()) {
                    qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start saveRoomSettingsJob";
                }
            }
        }
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
    case RoomHeaderWidget::FollowMessages:
        slotFollowMessages();
        break;
    }
}

void RoomWidget::slotPruneMessages()
{
    QPointer<PruneMessagesDialog> dlg = new PruneMessagesDialog(this);
    dlg->setRoomName(mRoom->name());
    if (dlg->exec()) {
        RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo info = dlg->cleanHistoryInfo();
        info.roomId = mRoomWidgetBase->roomId();
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
        info.roomId = mRoomWidgetBase->roomId();
        mCurrentRocketChatAccount->exportMessages(info);
    }
    delete dlg;
}

void RoomWidget::slotVideoChat()
{
    mCurrentRocketChatAccount->createJitsiConfCall(mRoomWidgetBase->roomId());
}

void RoomWidget::slotAddUsersInRoom()
{
    QPointer<AddUsersInRoomDialog> dlg = new AddUsersInRoomDialog(this);
    if (dlg->exec()) {
        const QStringList users = dlg->users();
        for (const QString &user : users) {
            mCurrentRocketChatAccount->addUserToRoom(user, mRoomWidgetBase->roomId(), mRoomType);
        }
    }
    delete dlg;
}

void RoomWidget::slotInviteUsers()
{
    QPointer<InviteUsersDialog> dlg = new InviteUsersDialog(this);
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->generateLink();
    dlg->exec();
    delete dlg;
}

void RoomWidget::updateListView()
{
    mRoomWidgetBase->updateListView();
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
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::StarredMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mRoomWidgetBase->messageListView(), &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotPinnedMessages()
{
    QPointer<ShowPinnedMessagesDialog> dlg = new ShowPinnedMessagesDialog(this);
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoom(mRoom);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::PinnedMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mRoomWidgetBase->messageListView(), &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowMentions()
{
    QPointer<ShowMentionsMessagesDialog> dlg = new ShowMentionsMessagesDialog(this);
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::MentionsMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mRoomWidgetBase->messageListView(), &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotSnipperedMessages()
{
    QPointer<ShowSnipperedMessagesDialog> dlg = new ShowSnipperedMessagesDialog(this);
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::SnipperedMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mRoomWidgetBase->messageListView(), &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowThreads()
{
    QPointer<ShowThreadsDialog> dlg = new ShowThreadsDialog(this);
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::ThreadsMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, mRoomWidgetBase->messageListView(), &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowDiscussions()
{
    QPointer<ShowDiscussionsDialog> dlg = new ShowDiscussionsDialog(this);
    dlg->setModel(mCurrentRocketChatAccount->discussionsFilterProxyModel());
    dlg->setRoomId(mRoomWidgetBase->roomId());
    mCurrentRocketChatAccount->discussionsInRoom(mRoomWidgetBase->roomId());
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowFileAttachments()
{
    QPointer<ShowAttachmentDialog> dlg = new ShowAttachmentDialog(this);
    mCurrentRocketChatAccount->roomFiles(mRoomWidgetBase->roomId(), mRoomType);
    dlg->setModel(mCurrentRocketChatAccount->filesForRoomFilterProxyModel());
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setRoomType(mRoomType);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotSearchMessages()
{
    QPointer<SearchMessageDialog> dlg = new SearchMessageDialog(this);
    dlg->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setRoom(mRoom);
    dlg->setModel(mCurrentRocketChatAccount->searchMessageFilterProxyModel());
    connect(dlg, &SearchMessageDialog::goToMessageRequested, mRoomWidgetBase->messageListView(), &MessageListView::goToMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotCreateNewDiscussion(const QString &messageId, const QString &originalMessage)
{
    mRoomWidgetBase->slotCreateNewDiscussion(messageId, originalMessage, mRoomHeaderWidget->roomName());
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
        mRoomWidgetBase->messageLineWidget()->handleMimeData(mimeData);
    }
}

void RoomWidget::storeRoomSettings()
{
    if (mCurrentRocketChatAccount) {
        if (mRoomWidgetBase->messageLineWidget()->text().isEmpty()) {
            auto *vbar = mRoomWidgetBase->messageListView()->verticalScrollBar();
            if (vbar->value() != vbar->maximum()) {
                AccountRoomSettings::PendingTypedInfo info;
                info.scrollbarPosition = mRoomWidgetBase->messageListView()->verticalScrollBar()->value();
                mCurrentRocketChatAccount->accountRoomSettings()->add(mRoomWidgetBase->roomId(), info);
            } else {
                mCurrentRocketChatAccount->accountRoomSettings()->remove(mRoomWidgetBase->roomId());
            }
        } else {
            AccountRoomSettings::PendingTypedInfo info;
            info.text = mRoomWidgetBase->messageLineWidget()->text();
            info.messageIdBeingEdited = mRoomWidgetBase->messageLineWidget()->messageIdBeingEdited();
            info.scrollbarPosition = mRoomWidgetBase->messageListView()->verticalScrollBar()->value();
            info.threadMessageId = mRoomWidgetBase->messageLineWidget()->threadMessageId();
            info.quotePermalink = mRoomWidgetBase->messageLineWidget()->quotePermalink();
            info.quoteText = mRoomWidgetBase->messageLineWidget()->quoteText();
            mCurrentRocketChatAccount->accountRoomSettings()->add(mRoomWidgetBase->roomId(), info);
        }
    }
}

void RoomWidget::clearBeforeSwitching()
{
    mRoomWidgetBase->messageLineWidget()->setThreadMessageId({});
    mRoomWidgetBase->messageLineWidget()->setQuoteMessage({}, {});
}

void RoomWidget::setChannelSelected(const QString &roomId, const QString &roomType)
{
    storeRoomSettings();
    setRoomId(roomId);
    setRoomType(roomType);
    clearBeforeSwitching();
    const AccountRoomSettings::PendingTypedInfo currentPendingInfo = mCurrentRocketChatAccount->accountRoomSettings()->value(roomId);
    if (currentPendingInfo.isValid()) {
        mRoomWidgetBase->messageLineWidget()->setQuoteMessage(currentPendingInfo.quotePermalink, currentPendingInfo.quoteText);
        mRoomWidgetBase->messageLineWidget()->setThreadMessageId(currentPendingInfo.threadMessageId);
        mRoomWidgetBase->messageLineWidget()->setText(currentPendingInfo.text);
        mRoomWidgetBase->messageLineWidget()->setMessageIdBeingEdited(currentPendingInfo.messageIdBeingEdited);
        if (currentPendingInfo.scrollbarPosition != -1) {
            mRoomWidgetBase->messageListView()->verticalScrollBar()->setValue(currentPendingInfo.scrollbarPosition);
        }
    } else {
        mRoomWidgetBase->messageLineWidget()->setText(QString());
    }
    mRoomWidgetBase->messageLineWidget()->setMode(mRoomWidgetBase->messageLineWidget()->messageIdBeingEdited().isEmpty()
                                                      ? MessageLineWidget::EditingMode::NewMessage
                                                      : MessageLineWidget::EditingMode::EditMessage);

    mRoomWidgetBase->messageLineWidget()->setFocus();
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
        mRoomHeaderWidget->setRoomAnnouncement(mRoom->displayAnnouncement());
        mRoomHeaderWidget->setRoomTopic(mRoom->displayTopic());
        mRoomHeaderWidget->setFavoriteStatus(mRoom->favorite());
        mRoomHeaderWidget->setEncypted(mRoom->encrypted() && mRoom->hasPermission(QStringLiteral("edit-room")));
        mRoomHeaderWidget->setIsDiscussion(mRoom->isDiscussionRoom());
        // TODO Description ?

        mRoomWidgetBase->updateRoomReadOnly(mRoom);
        if (mRoom->channelCounterInfo().isValid() && mRoom->channelCounterInfo().unreadMessages() > 0) {
            mRoomCounterInfoWidget->animatedShow();
        } else {
            mRoomCounterInfoWidget->animatedHide();
        }
    }
}

QString RoomWidget::roomId() const
{
    return mRoomWidgetBase->roomId();
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
    mRoomWidgetBase->setRoomId(roomId);
    mRoom = mCurrentRocketChatAccount->room(mRoomWidgetBase->roomId());
    if (mRoom) {
        connectRoom();
        mRoomWidgetBase->messageLineWidget()->setRoomId(roomId);
        mRoomWidgetBase->messageListView()->setChannelSelected(mRoom);
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
            mRoomHeaderWidget->setRoomAnnouncement(mRoom->displayAnnouncement());
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
        connect(mRoom, &Room::highlightsWordChanged, this, &RoomWidget::updateListView);
    }
    slotUpdateRoomCounterInfoWidget();
    updateRoomHeader();
}

void RoomWidget::slotJumpToUnreadMessage(qint64 numberOfMessage)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    MessageModel *roomMessageModel = rcAccount->messageModelForRoom(mRoomWidgetBase->roomId());
    if (roomMessageModel->rowCount() > numberOfMessage) {
        const QString messageId = roomMessageModel->messageIdFromIndex(roomMessageModel->rowCount() - numberOfMessage);
        mRoomWidgetBase->messageListView()->goToMessage(messageId);
    } else {
        RocketChatRestApi::ChannelHistoryJob *job = new RocketChatRestApi::ChannelHistoryJob(this);
        RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo info;
        // TODO verify it.
        info.channelType = mRoomType == QLatin1String("c") ? RocketChatRestApi::ChannelHistoryJob::Channel : RocketChatRestApi::ChannelHistoryJob::Groups;
        info.count = numberOfMessage;
        info.roomId = mRoomWidgetBase->roomId();
        const qint64 endDateTime = roomMessageModel->lastTimestamp();
        info.latestMessage = QDateTime::fromMSecsSinceEpoch(endDateTime).toString(Qt::ISODateWithMs);
        // qDebug() << " info.latestMessage " << info.latestMessage;
        job->setChannelHistoryInfo(info);
        rcAccount->restApi()->initializeRestApiJob(job);
        connect(
            job,
            &RocketChatRestApi::ChannelHistoryJob::channelHistoryDone,
            this,
            [this, numberOfMessage, rcAccount, roomMessageModel](const QJsonObject &obj, const RocketChatRestApi::ChannelBaseJob::ChannelInfo &channelInfo) {
                rcAccount->rocketChatBackend()->processIncomingMessages(obj.value(QLatin1String("messages")).toArray(), false, true);
                // qDebug() << " obj " << obj;
                // qDebug() << " initialRowCount " <<  (roomMessageModel->rowCount() - numberOfMessage);

                const QString messageId = roomMessageModel->messageIdFromIndex(roomMessageModel->rowCount() - numberOfMessage);
                mRoomWidgetBase->messageListView()->goToMessage(messageId);
            });
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelHistoryJob";
        }
    }
}

void RoomWidget::slotClearNotification()
{
    mCurrentRocketChatAccount->markRoomAsRead(mRoomWidgetBase->roomId());
}

void RoomWidget::slotEncryptedChanged(bool b)
{
    qCWarning(RUQOLAWIDGETS_LOG) << "change encrypted not supported yet";
    // TODO mCurrentRocketChatAccount->slot
}

void RoomWidget::slotChangeFavorite(bool b)
{
    mCurrentRocketChatAccount->changeFavorite(mRoomWidgetBase->roomId(), b);
}

QString RoomWidget::roomType() const
{
    return mRoomType;
}

void RoomWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::openThreadRequested, this, &RoomWidget::slotOpenThreadRequested);
    }

    mCurrentRocketChatAccount = account;
    mRoomWidgetBase->setCurrentRocketChatAccount(account);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openThreadRequested, this, &RoomWidget::slotOpenThreadRequested);
    mRoomHeaderWidget->setCurrentRocketChatAccount(account);
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

void RoomWidget::slotFollowMessages()
{
    qDebug() << " RoomWidget::slotFollowMessages() not implemented yet";
}

void RoomWidget::setLayoutSpacing(int spacing)
{
    mRoomWidgetBase->layout()->setSpacing(spacing);
}
