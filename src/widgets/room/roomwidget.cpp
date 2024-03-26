/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomwidget.h"
#include "accountroomsettings.h"
#include "conferencecalldialog/conferencecalldialog.h"
#include "connection.h"
#include "dialogs/addusersinroomdialog.h"
#include "dialogs/autotranslateconfiguredialog.h"
#include "dialogs/channelinfodialog.h"
#include "dialogs/configurenotificationdialog.h"
#include "dialogs/directchannelinfodialog.h"
#include "dialogs/inviteusersdialog.h"
#include "dialogs/searchmessagedialog.h"
#include "dialogs/showattachmentdialog.h"
#include "dialogs/showmentionsmessagesdialog.h"
#include "dialogs/showpinnedmessagesdialog.h"
#include "dialogs/showstarredmessagesdialog.h"
#include "dialogs/showthreadsdialog.h"
#include "discussions/showdiscussionsdialog.h"
#include "exportmessages/exportmessagesdialog.h"
#include "messagelinewidget.h"
#include "messagelistview.h"
#include "plugintextmessagewidget.h"
#include "prunemessages/prunemessagesdialog.h"
#include "rocketchataccount.h"
#include "rocketchatbackend.h"
#include "roomutil.h"
#include "ruqolawidgets_debug.h"
#include "usersinroomflowwidget.h"

#include "offlinewidget/offlinewidget.h"

#include "otr/otrwidget.h"
#include "reconnectinfowidget.h"
#include "roomcounterinfowidget.h"
#include "roomwidgetbase.h"
#include "ruqola_thread_message_widgets_debug.h"
#include "teams/teamchannelsdialog.h"
#include "teams/teaminfo.h"
#include "threadwidget/threadmessagedialog.h"
#include "video-conference/videoconferencejoinjob.h"
#include "video-conference/videoconferencestartjob.h"
#include "videoconference/videoconferencemessageinfomanager.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPushButton>
#include <QScrollBar>

#include "config-ruqola.h"
#include "video-conference/videoconferencecapabilitiesjob.h"

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

RoomWidget::RoomWidget(QWidget *parent)
    : QWidget(parent)
    , mRoomWidgetBase(new RoomWidgetBase(MessageListView::Mode::Editing, this))
    , mRoomHeaderWidget(new RoomHeaderWidget(this))
    , mUsersInRoomFlowWidget(new UsersInRoomFlowWidget(this))
    , mRoomCounterInfoWidget(new RoomCounterInfoWidget(this))

#if HAVE_TEXT_TO_SPEECH
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mRoomHeaderWidget->setObjectName(QStringLiteral("mRoomHeaderWidget"));
    mainLayout->addWidget(mRoomHeaderWidget);

    auto roomWidget = new QWidget(this);
    mainLayout->addWidget(roomWidget);
    mRoomWidgetLayout = new QVBoxLayout(roomWidget);
    mRoomWidgetLayout->setObjectName(QStringLiteral("roomWidgetLayout"));
    mRoomWidgetLayout->setContentsMargins({});
    mRoomWidgetLayout->setSpacing(0);

    mUsersInRoomFlowWidget->setObjectName(QStringLiteral("mUsersInRoomFlowWidget"));
    mRoomWidgetLayout->addWidget(mUsersInRoomFlowWidget);
    mUsersInRoomFlowWidget->setVisible(false);

    mRoomCounterInfoWidget->setObjectName(QStringLiteral("mRoomCounterInfoWidget"));

    connect(mRoomWidgetBase, &RoomWidgetBase::errorMessage, this, [this](const QString &message) {
        if (!mPluginTextMessageWidget) {
            createPluginTextMessateWidget();
        }
        mPluginTextMessageWidget->slotShareError(message);
    });
    connect(mRoomWidgetBase, &RoomWidgetBase::successMessage, this, [this](const QString &message) {
        if (!mPluginTextMessageWidget) {
            createPluginTextMessateWidget();
        }
        mPluginTextMessageWidget->slotShareSuccess(message);
    });

    mRoomWidgetLayout->addWidget(mRoomCounterInfoWidget);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mRoomWidgetLayout->addWidget(mTextToSpeechWidget);
    connect(mRoomWidgetBase, &RoomWidgetBase::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif

    mRoomWidgetLayout->addWidget(mRoomWidgetBase);
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
    connect(mRoomHeaderWidget, &RoomHeaderWidget::teamChannelsRequested, this, &RoomWidget::slotTeamChannelsRequested);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::openTeam, this, &RoomWidget::slotOpenTeamRequested);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::callRequested, this, &RoomWidget::slotCallRequested);
    setAcceptDrops(true);
}

RoomWidget::~RoomWidget()
{
    delete mRoom;
}

void RoomWidget::createPluginTextMessateWidget()
{
    mPluginTextMessageWidget = new PluginTextMessageWidget(this);
    mPluginTextMessageWidget->setObjectName(QStringLiteral("mPluginTextMessageWidget"));
    mRoomWidgetLayout->insertWidget(1, mPluginTextMessageWidget);
}

// TODO using it
void RoomWidget::createRoomReconnectInfoWidget()
{
    mRoomReconnectInfoWidget = new ReconnectInfoWidget(this);
    mRoomReconnectInfoWidget->setObjectName(QStringLiteral("mRoomReconnectInfoWidget"));
    connect(mRoomReconnectInfoWidget, &ReconnectInfoWidget::tryReconnect, this, &RoomWidget::slotTryReconnect);
    // After mUsersInRoomFlowWidget
    mRoomWidgetLayout->insertWidget(1, mRoomReconnectInfoWidget);
}

// TODO using it.
void RoomWidget::createOffLineWidget()
{
    mOffLineWidget = new OffLineWidget(this);
    mOffLineWidget->setObjectName(QStringLiteral("mOffLineWidget"));
    // After mUsersInRoomFlowWidget
    mRoomWidgetLayout->insertWidget(1, mOffLineWidget);
}

// TODO using it.
void RoomWidget::createOtrWidget()
{
    mOtrWidget = new OtrWidget(this);
    mOtrWidget->setObjectName(QStringLiteral("mOtrWidget"));
    connect(mOtrWidget, &OtrWidget::closeOtr, this, &RoomWidget::slotCloseOtr);
    connect(mOtrWidget, &OtrWidget::refreshKeys, this, &RoomWidget::slotRefreshOtrKeys);
    // After mUsersInRoomFlowWidget
    mRoomWidgetLayout->insertWidget(1, mOtrWidget);
}

void RoomWidget::slotLoadHistory()
{
    mCurrentRocketChatAccount->loadHistory(mRoomWidgetBase->roomId());
}

void RoomWidget::slotChannelInfoRequested()
{
    if (!mRoom) {
        return;
    }
    if (mRoomType == Room::RoomType::Direct) {
        DirectChannelInfoDialog dlg(mCurrentRocketChatAccount, this);
        dlg.setUserName(mRoom->name());
        dlg.exec();
    } else {
        QPointer<ChannelInfoDialog> dlg = new ChannelInfoDialog(mRoom, mCurrentRocketChatAccount, this);
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
    case RoomHeaderWidget::OtrMessages:
        // TODO
        break;
    case RoomHeaderWidget::EncryptMessages:
        // TODO
        break;
    }
}

void RoomWidget::slotPruneMessages()
{
    if (!mRoom) {
        return;
    }
    QPointer<PruneMessagesDialog> dlg = new PruneMessagesDialog(mCurrentRocketChatAccount, this);
    dlg->setRoomName(mRoom->name());
    if (dlg->exec()) {
        RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo info = dlg->cleanHistoryInfo();
        info.roomId = QString::fromLatin1(mRoomWidgetBase->roomId());
        if (KMessageBox::ButtonCode::PrimaryAction
            == KMessageBox::questionTwoActions(this,
                                               i18n("Do you want really remove history?"),
                                               i18nc("@title:window", "Remove History"),
                                               KStandardGuiItem::remove(),
                                               KStandardGuiItem::cancel())) {
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
        info.roomId = QString::fromLatin1(mRoomWidgetBase->roomId());
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
    QPointer<AddUsersInRoomDialog> dlg = new AddUsersInRoomDialog(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const QStringList users = dlg->userIds();
        for (const QString &user : users) {
            mCurrentRocketChatAccount->addUserToRoom(user, mRoomWidgetBase->roomId(), mRoomType);
        }
    }
    delete dlg;
}

void RoomWidget::slotInviteUsers()
{
    InviteUsersDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoomId(QString::fromLatin1(mRoomWidgetBase->roomId()));
    dlg.generateLink();
    dlg.exec();
}

void RoomWidget::updateListView()
{
    mRoomWidgetBase->updateListView();
}

void RoomWidget::slotConfigureAutoTranslate()
{
    if (!mRoom) {
        return;
    }
    AutoTranslateConfigureDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoom(mRoom);
    dlg.exec();
}

void RoomWidget::slotConfigureNotification()
{
    if (!mRoom) {
        return;
    }
    ConfigureNotificationDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoom(mRoom);
    dlg.exec();
}

void RoomWidget::slotStarredMessages()
{
    if (!mRoom) {
        return;
    }
    QPointer<ShowStarredMessagesDialog> dlg = new ShowStarredMessagesDialog(mCurrentRocketChatAccount, this);
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::StarredMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, this, &RoomWidget::slotGotoMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotPinnedMessages()
{
    if (!mRoom) {
        return;
    }
    QPointer<ShowPinnedMessagesDialog> dlg = new ShowPinnedMessagesDialog(mCurrentRocketChatAccount, this);
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setRoom(mRoom);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::PinnedMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, this, &RoomWidget::slotGotoMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowMentions()
{
    if (!mRoom) {
        return;
    }
    QPointer<ShowMentionsMessagesDialog> dlg = new ShowMentionsMessagesDialog(mCurrentRocketChatAccount, this);
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::MentionsMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, this, &RoomWidget::slotGotoMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowThreads()
{
    if (!mRoom) {
        return;
    }
    QPointer<ShowThreadsDialog> dlg = new ShowThreadsDialog(mCurrentRocketChatAccount, this);
    dlg->setModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(mRoomWidgetBase->roomId(), ListMessagesModel::ThreadsMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, this, &RoomWidget::slotGotoMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowDiscussions()
{
    auto dlg = new ShowDiscussionsDialog(mCurrentRocketChatAccount, this);
    dlg->setModel(mCurrentRocketChatAccount->discussionsFilterProxyModel());
    dlg->setRoomId(mRoomWidgetBase->roomId());
    mCurrentRocketChatAccount->discussionsInRoom(mRoomWidgetBase->roomId());
    dlg->show();
}

void RoomWidget::slotShowFileAttachments()
{
    auto dlg = new ShowAttachmentDialog(mCurrentRocketChatAccount, this);
    mCurrentRocketChatAccount->roomFiles(mRoomWidgetBase->roomId(), mRoomType);
    dlg->setModel(mCurrentRocketChatAccount->filesForRoomFilterProxyModel());
    dlg->setRoomId(mRoomWidgetBase->roomId());
    dlg->setRoomType(mRoomType);
    dlg->show();
}

void RoomWidget::slotSearchMessages()
{
    if (!mRoom) {
        return;
    }
    SearchMessageDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoomId(mRoomWidgetBase->roomId());
    dlg.setRoom(mRoom);
    dlg.setModel(mCurrentRocketChatAccount->searchMessageFilterProxyModel());
    connect(&dlg, &SearchMessageDialog::goToMessageRequested, this, &RoomWidget::slotGotoMessage);
    dlg.exec();
}

void RoomWidget::slotCallRequested()
{
    if (!mRoom) {
        return;
    }
    auto job = new RocketChatRestApi::VideoConferenceCapabilitiesJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::VideoConferenceCapabilitiesJob::noVideoConferenceProviderApps, this, [this] {
        KMessageBox::information(this, i18n("A workspace admin needs to install and configure a conference call apps."), i18n("Video Conference"));
    });
    connect(job, &RocketChatRestApi::VideoConferenceCapabilitiesJob::videoConferenceCapabilitiesDone, this, [this](const QJsonObject &obj) {
        // qDebug() << "obj  " << obj;
        // {"capabilities":{"cam":true,"mic":true,"title":true},"providerName":"jitsi","success":true}
        const QJsonObject capabilitiesObj = obj[QLatin1StringView("capabilities")].toObject();
        const bool useCam = capabilitiesObj[QLatin1StringView("cam")].toBool();
        const bool useMic = capabilitiesObj[QLatin1StringView("mic")].toBool();
        ConferenceCallWidget::ConferenceCallStart callInfo;
        callInfo.useCamera = useCam;
        callInfo.useMic = useMic;

        QPointer<ConferenceCallDialog> dlg = new ConferenceCallDialog(mCurrentRocketChatAccount, this);
        dlg->setConferenceCallInfo(callInfo);
        if (dlg->exec()) {
            const ConferenceCallWidget::ConferenceCallStart conferenceCallInfo = dlg->conferenceCallInfo();

            auto job = new RocketChatRestApi::VideoConferenceStartJob(this);
            RocketChatRestApi::VideoConferenceStartJob::VideoConferenceStartInfo startInfo;
            startInfo.roomId = QString::fromLatin1(mRoomWidgetBase->roomId());
            startInfo.allowRinging = mRoom->hasPermission(QStringLiteral("videoconf-ring-users"));
            job->setInfo(startInfo);
            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::VideoConferenceStartJob::videoConferenceStartDone, this, [this, conferenceCallInfo](const QJsonObject &obj) {
                // qDebug() << "obj  " << obj;
                // {"data":{"callId":"63949ea24ef3f3baa9658f25","providerName":"jitsi","rid":"hE6RS3iv5ND5EGWC6","type":"videoconference"},"success":true}
                const QString callId{obj[QLatin1StringView("callId")].toString()};
                mCurrentRocketChatAccount->videoConferenceMessageInfoManager()->addCallId(callId);
                auto conferenceJoinJob = new RocketChatRestApi::VideoConferenceJoinJob(this);
                RocketChatRestApi::VideoConferenceJoinJob::VideoConferenceJoinInfo joinInfo;
                joinInfo.callId = callId;
                joinInfo.useCamera = conferenceCallInfo.useCamera;
                joinInfo.useMicro = conferenceCallInfo.useMic;
                conferenceJoinJob->setInfo(joinInfo);
                mCurrentRocketChatAccount->restApi()->initializeRestApiJob(conferenceJoinJob);
                connect(conferenceJoinJob, &RocketChatRestApi::VideoConferenceJoinJob::videoConferenceJoinDone, this, [](const QJsonObject &joinObject) {
                    // qDebug() << " join info " << obj;
                    QDesktopServices::openUrl(QUrl(joinObject[QLatin1StringView("url")].toString()));
                });
                if (!conferenceJoinJob->start()) {
                    qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceJoinJob job";
                }
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceCapabilitiesJob job";
            }
        }
        delete dlg;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceCapabilitiesJob job";
    }
}

void RoomWidget::slotOpenTeamRequested(const QByteArray &teamId)
{
    Q_EMIT mCurrentRocketChatAccount->openTeamNameRequested(teamId);
}

void RoomWidget::slotTeamChannelsRequested()
{
    if (!mRoom) {
        return;
    }
    TeamChannelsDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoom(mRoom);
    dlg.exec();
}

void RoomWidget::slotCreateNewDiscussion(const QByteArray &messageId, const QString &originalMessage)
{
    mRoomWidgetBase->slotCreateNewDiscussion(messageId, originalMessage, mRoomHeaderWidget->roomName());
}

void RoomWidget::slotCreatePrivateDiscussion(const QString &userName)
{
    Q_EMIT mCurrentRocketChatAccount->openLinkRequested(RoomUtil::generateUserLink(userName));
}

void RoomWidget::dragEnterEvent(QDragEnterEvent *event)
{
    // Don't allow to drop element when it's blocked
    if (mRoom && mRoom->roomIsBlocked()) {
        return;
    }
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
            info.messageIdBeingEdited = QString::fromLatin1(mRoomWidgetBase->messageLineWidget()->messageIdBeingEdited());
            info.scrollbarPosition = mRoomWidgetBase->messageListView()->verticalScrollBar()->value();
            info.threadMessageId = QString::fromLatin1(mRoomWidgetBase->messageLineWidget()->threadMessageId());
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

void RoomWidget::forceLineEditFocus()
{
    mRoomWidgetBase->messageLineWidget()->setFocus();
}

void RoomWidget::setChannelSelected(const QByteArray &roomId, Room::RoomType roomType)
{
    storeRoomSettings();
    setRoomId(roomId);
    setRoomType(roomType);
    clearBeforeSwitching();
    const AccountRoomSettings::PendingTypedInfo currentPendingInfo = mCurrentRocketChatAccount->accountRoomSettings()->value(roomId);
    if (currentPendingInfo.isValid()) {
        mRoomWidgetBase->messageLineWidget()->setQuoteMessage(currentPendingInfo.quotePermalink, currentPendingInfo.quoteText);
        mRoomWidgetBase->messageLineWidget()->setThreadMessageId(currentPendingInfo.threadMessageId.toLatin1());
        mRoomWidgetBase->messageLineWidget()->setText(currentPendingInfo.text);
        mRoomWidgetBase->messageLineWidget()->setMessageIdBeingEdited(currentPendingInfo.messageIdBeingEdited.toLatin1());
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
        mRoomHeaderWidget->setIsMainTeam(mRoom->teamInfo().mainTeam());
        mRoomHeaderWidget->setTeamRoomInfo(mRoom->teamRoomInfo());
        mRoomHeaderWidget->setIsDirectGroup((mRoom->channelType() == Room::RoomType::Direct) && mRoom->userNames().count() > 2);
        if (mRoom->roomId() == QByteArray((mCurrentRocketChatAccount->userId() + mCurrentRocketChatAccount->userId()))) {
            mRoomHeaderWidget->setCallEnabled(false);
        } else {
            mRoomHeaderWidget->setCallEnabled(true);
        }
        // TODO Description ?

        mRoomWidgetBase->updateRoomReadOnly(mRoom);
        if (mRoom->channelCounterInfo().isValid() && mRoom->channelCounterInfo().unreadMessages() > 0) {
            mRoomCounterInfoWidget->animatedShow();
        } else {
            mRoomCounterInfoWidget->animatedHide();
        }
    }
}

QByteArray RoomWidget::roomId() const
{
    return mRoomWidgetBase->roomId();
}

void RoomWidget::setRoomType(Room::RoomType roomType)
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

void RoomWidget::setRoomId(const QByteArray &roomId)
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
        connect(mRoom, &Room::fnameChanged, this, [this]() {
            mRoomHeaderWidget->setRoomName(mRoom->displayRoomName());
        });
        connect(mRoom, &Room::favoriteChanged, this, [this]() {
            mRoomHeaderWidget->setFavoriteStatus(mRoom->favorite());
        });
        connect(mRoom, &Room::encryptedChanged, this, [this]() {
            mRoomHeaderWidget->setEncypted(mRoom->encrypted());
        });
        // TODO verify it.
        connect(mRoom, &Room::teamInfoChanged, this, [this]() {
            mRoomHeaderWidget->setIsMainTeam(mRoom->teamInfo().mainTeam());
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
    MessagesModel *roomMessageModel = mCurrentRocketChatAccount->messageModelForRoom(mRoomWidgetBase->roomId());
    if (roomMessageModel->rowCount() >= numberOfMessage) {
        const QByteArray messageId = roomMessageModel->messageIdFromIndex(roomMessageModel->rowCount() - numberOfMessage);
        mRoomWidgetBase->messageListView()->goToMessage(messageId);
    } else {
        RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo info;
        switch (mRoomType) {
        case Room::RoomType::Channel:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::Channel;
            break;
        case Room::RoomType::Direct:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::Direct;
            break;
        case Room::RoomType::Private:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::Groups;
            break;
        case Room::RoomType::Unknown:
            qCWarning(RUQOLAWIDGETS_LOG) << " Problem with room type ";
            break;
        }
        if (mRoomType == Room::RoomType::Unknown) {
            return;
        }
        auto job = new RocketChatRestApi::ChannelHistoryJob(this);
        info.count = numberOfMessage - roomMessageModel->rowCount() + 1;
        info.roomId = QString::fromLatin1(mRoomWidgetBase->roomId());
        const qint64 endDateTime = roomMessageModel->lastTimestamp();
        info.latestMessage = QDateTime::fromMSecsSinceEpoch(endDateTime, Qt::UTC).toString(Qt::ISODateWithMs);
        // qDebug() << " info.latestMessage " << info.latestMessage;
        job->setChannelHistoryInfo(info);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::ChannelHistoryJob::channelHistoryDone, this, [this, numberOfMessage, roomMessageModel](const QJsonObject &obj) {
            mCurrentRocketChatAccount->rocketChatBackend()->processIncomingMessages(obj.value(QLatin1StringView("messages")).toArray(), true, true);
            // qDebug() << " obj " << obj;
            //                qDebug() << " roomMessageModel->rowCount() " << roomMessageModel->rowCount();
            //                qDebug() << " numberOfMessage " << numberOfMessage;
            //                qDebug() << " initialRowCount " <<  (roomMessageModel->rowCount() - numberOfMessage);

            const QByteArray messageId = roomMessageModel->messageIdFromIndex(roomMessageModel->rowCount() - numberOfMessage);
            mRoomWidgetBase->messageListView()->goToMessage(messageId);
        });
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelHistoryJob";
        }
    }
}

void RoomWidget::scrollToMessageId(const QByteArray &messageId)
{
    slotGotoMessage(messageId, {});
}

void RoomWidget::slotGotoMessage(const QByteArray &messageId, const QString &messageDateTimeUtc)
{
    MessageListView *messageListView = mRoomWidgetBase->messageListView();
    auto messageModel = qobject_cast<MessagesModel *>(messageListView->model());
    Q_ASSERT(messageModel);
    const QModelIndex index = messageModel->indexForMessage(messageId);
    if (index.isValid()) {
        messageListView->scrollTo(index);
    } else /* if (!messageDateTimeUtc.isEmpty())*/ {
        RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo info;
        switch (mRoomType) {
        case Room::RoomType::Channel:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::Channel;
            break;
        case Room::RoomType::Direct:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::Direct;
            break;
        case Room::RoomType::Private:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::Groups;
            break;
        case Room::RoomType::Unknown:
            qCWarning(RUQOLAWIDGETS_LOG) << " Problem with room type ";
            break;
        }
        if (mRoomType == Room::RoomType::Unknown) {
            return;
        }
        auto job = new RocketChatRestApi::ChannelHistoryJob(this);
        info.roomId = QString::fromLatin1(mRoomWidgetBase->roomId());
        const qint64 endDateTime = messageModel->lastTimestamp();
        info.latestMessage = QDateTime::fromMSecsSinceEpoch(endDateTime, Qt::UTC).toString(Qt::ISODateWithMs);
        info.oldestMessage = messageDateTimeUtc;
        info.inclusive = true;
        info.count = 5000;
        // qDebug() << " info " << info;
        job->setChannelHistoryInfo(info);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::ChannelHistoryJob::channelHistoryDone, this, [messageId, messageModel, messageListView, this](const QJsonObject &obj) {
            mCurrentRocketChatAccount->rocketChatBackend()->processIncomingMessages(obj.value(QLatin1StringView("messages")).toArray(), true, true);
            const QModelIndex index = messageModel->indexForMessage(messageId);
            if (index.isValid()) {
                messageListView->scrollTo(index);
            } else {
                qCWarning(RUQOLAWIDGETS_LOG) << "Message not found:" << messageId;
            }
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
    RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo info;
    info.encrypted = b;
    info.roomId = QString::fromLatin1(mRoomWidgetBase->roomId());
    info.roomType = mRoom ? mRoom->roomFromRoomType(mRoom->channelType()) : QString();
    info.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::Encrypted;
    auto saveRoomSettingsJob = new RocketChatRestApi::SaveRoomSettingsJob(this);
    saveRoomSettingsJob->setSaveRoomSettingsInfo(info);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(saveRoomSettingsJob);
    if (!saveRoomSettingsJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start saveRoomSettingsJob";
    }
}

void RoomWidget::slotChangeFavorite(bool b)
{
    mCurrentRocketChatAccount->changeFavorite(mRoomWidgetBase->roomId(), b);
}

Room::RoomType RoomWidget::roomType() const
{
    return mRoomType;
}

void RoomWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::openThreadRequested, this, &RoomWidget::slotOpenThreadRequested);
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::displayReconnectWidget, this, &RoomWidget::slotDisplayReconnectWidget);
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RoomWidget::slotLoginStatusChanged);
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::needUpdateMessageView, this, &RoomWidget::updateListView);
    }

    mCurrentRocketChatAccount = account;
    mRoomWidgetBase->setCurrentRocketChatAccount(account);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::openThreadRequested, this, &RoomWidget::slotOpenThreadRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::displayReconnectWidget, this, &RoomWidget::slotDisplayReconnectWidget);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RoomWidget::slotLoginStatusChanged);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::needUpdateMessageView, this, &RoomWidget::updateListView);
    // TODO verify if we need to show or not reconnect widget
    mRoomHeaderWidget->setCurrentRocketChatAccount(account);
    mUsersInRoomFlowWidget->setCurrentRocketChatAccount(account);
}

void RoomWidget::slotLoginStatusChanged()
{
    const auto loginStatus = mCurrentRocketChatAccount->loginStatus();
    if (loginStatus == AuthenticationManager::LoggedIn) {
        if (mRoomReconnectInfoWidget) {
            mRoomReconnectInfoWidget->hide();
        }
    }
}

void RoomWidget::slotGoBackToRoom()
{
    if (mRoom) {
        Q_EMIT selectChannelRequested(mRoom->parentRid());
    }
}

void RoomWidget::slotOpenThreadRequested(const QByteArray &threadMessageId,
                                         const QString &threadMessagePreview,
                                         bool threadIsFollowing,
                                         const Message &threadMessage)
{
    qCDebug(RUQOLA_THREAD_MESSAGE_WIDGETS_LOG) << "threadMessageId: " << threadMessageId;
    auto dlg = new ThreadMessageDialog(mCurrentRocketChatAccount, this);
    ThreadMessageWidget::ThreadMessageInfo info;
    info.threadMessageId = threadMessageId;
    info.threadMessagePreview = threadMessagePreview;
    info.threadIsFollowing = threadIsFollowing;
    info.room = mRoom;
    info.messageThread = threadMessage;
    dlg->setThreadMessageInfo(info);
    dlg->show();
}

void RoomWidget::setLayoutSpacing(int spacing)
{
    mRoomWidgetBase->layout()->setSpacing(spacing);
}

void RoomWidget::slotTryReconnect()
{
    mCurrentRocketChatAccount->reconnectToServer();
}

void RoomWidget::slotDisplayReconnectWidget(int seconds)
{
    // Disable for the moment it seems to create some problems
    // FIXME mRoomReconnectInfoWidget->setReconnectSecondDelay(seconds);
}

void RoomWidget::slotCloseOtr()
{
    mCurrentRocketChatAccount->ddp()->streamNotifyUserOtrEnd(QString::fromLatin1(roomId()), QString::fromLatin1(mCurrentRocketChatAccount->userId()));
}

void RoomWidget::slotRefreshOtrKeys()
{
    // TODO
}

#include "moc_roomwidget.cpp"
