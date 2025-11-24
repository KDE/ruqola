/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomwidget.h"
#include "autogenerateui/autogenerateinteractionui.h"
#include "autogenerateui/autogenerateinteractionuidialog.h"
#include "encryption/e2ecopypassworddialog.h"
#include "encryption/e2edecodeencryptionkeyfailedwidget.h"
#include "encryption/e2edecodeencryptionkeywidget.h"
#include "encryption/e2ekeymanager.h"
#include "encryption/e2epassworddecodekeydialog.h"
#include "encryption/e2esaveencryptionkeywidget.h"

#include "accountroomsettings.h"
#include "conferencecalldialog/conferencecalldialog.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
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
#include "misc/methodcalljob.h"
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
#include "rocketchataccountsettings.h"
#include "roomcounterinfowidget.h"
#include "roomwidgetbase.h"
#include "ruqola_thread_message_widgets_debug.h"
#include "teams/teamchannelsdialog.h"
#include "teams/teaminfo.h"
#include "threadwidget/threadmessagedialog.h"
#include "video-conference/videoconferencejoinjob.h"
#include "video-conference/videoconferencestartjob.h"
#include "videoconference/videoconferencemessageinfomanager.h"

#include "rooms/roomscleanhistoryjob.h"
#include "webdav/webdavaddserverdialog.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <KNotification>

#include "ruqolautils.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPushButton>
#include <QScrollBar>
#include <QTimeZone>
#include <QVBoxLayout>

#include "config-ruqola.h"
#include "video-conference/videoconferencecapabilitiesjob.h"

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

using namespace Qt::Literals::StringLiterals;
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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mRoomHeaderWidget->setObjectName(u"mRoomHeaderWidget"_s);
    mainLayout->addWidget(mRoomHeaderWidget);

    auto roomWidget = new QWidget(this);
    mainLayout->addWidget(roomWidget);
    mRoomWidgetLayout = new QVBoxLayout(roomWidget);
    mRoomWidgetLayout->setObjectName(u"roomWidgetLayout"_s);
    mRoomWidgetLayout->setContentsMargins({});
    mRoomWidgetLayout->setSpacing(0);

    mUsersInRoomFlowWidget->setObjectName(u"mUsersInRoomFlowWidget"_s);
    mRoomWidgetLayout->addWidget(mUsersInRoomFlowWidget);
    mUsersInRoomFlowWidget->setVisible(false);

    mRoomCounterInfoWidget->setObjectName(u"mRoomCounterInfoWidget"_s);

    connect(mRoomWidgetBase, &RoomWidgetBase::errorMessage, this, [this](const QString &message) {
        if (!mPluginTextMessageWidget) {
            createPluginTextMessageWidget();
        }
        mPluginTextMessageWidget->slotShareError(message);
    });
    connect(mRoomWidgetBase, &RoomWidgetBase::successMessage, this, [this](const QString &message) {
        if (!mPluginTextMessageWidget) {
            createPluginTextMessageWidget();
        }
        mPluginTextMessageWidget->slotShareSuccess(message);
    });

    mRoomWidgetLayout->addWidget(mRoomCounterInfoWidget);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mRoomWidgetLayout->addWidget(mTextToSpeechWidget);
    connect(mRoomWidgetBase, &RoomWidgetBase::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::enqueue);
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
    connect(this, &RoomWidget::showUiInteractionDialog, this, &RoomWidget::displayUiInteractionDialog);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::uiInteractionRequested, this, &RoomWidget::displayUiInteractionDialog);
    connect(mRoomWidgetBase, &RoomWidgetBase::uiInteractionRequested, this, &RoomWidget::displayUiInteractionDialog);
    connect(mRoomWidgetBase, &RoomWidgetBase::addWebDavServer, this, &RoomWidget::slotAddWebDavServer);
    setAcceptDrops(true);
}

RoomWidget::~RoomWidget() = default;

void RoomWidget::slotAddWebDavServer()
{
    WebDavAddServerDialog d(this);
    if (d.exec()) {
        const WebDavAddServerWidget::WebDavAddServerInfo infoServerInfo = d.addServerInfo();
        // it uses "/api/v1/method.call/addWebdavAccount"
        // => use restapi for calling ddp method
        auto job = new RocketChatRestApi::MethodCallJob(this);
        RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
        info.methodName = u"addWebdavAccount"_s;
        info.anonymous = false;

        //[{\"name\":\"test1\",\"serverURL\":\"http://www.kde.org\",\"username\":\"A\",\"password\":\"A\"}]}
        QJsonObject obj;
        obj["name"_L1] = infoServerInfo.name;
        obj["serverURL"_L1] = infoServerInfo.url;
        obj["username"_L1] = infoServerInfo.userName;
        obj["password"_L1] = infoServerInfo.password;
        const QJsonArray params{obj};
        info.messageObj = mCurrentRocketChatAccount->ddp()->generateJsonObject(info.methodName, params);
        job->setMethodCallJobInfo(info);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
        connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [](const QJsonObject &replyObject) {
            qDebug() << " replyObject " << replyObject;
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start getRoomByTypeAndName job";
        }
    }
}

void RoomWidget::createPluginTextMessageWidget()
{
    mPluginTextMessageWidget = new PluginTextMessageWidget(this);
    mPluginTextMessageWidget->setObjectName(u"mPluginTextMessageWidget"_s);
    mRoomWidgetLayout->insertWidget(1, mPluginTextMessageWidget);
}

void RoomWidget::createRoomReconnectInfoWidget()
{
    mRoomReconnectInfoWidget = new ReconnectInfoWidget(this);
    mRoomReconnectInfoWidget->setObjectName(u"mRoomReconnectInfoWidget"_s);
    connect(mRoomReconnectInfoWidget, &ReconnectInfoWidget::tryReconnect, this, &RoomWidget::slotTryReconnect);
    // After mUsersInRoomFlowWidget
    mRoomWidgetLayout->insertWidget(1, mRoomReconnectInfoWidget);
}

// TODO using it.
void RoomWidget::createOffLineWidget()
{
    mOffLineWidget = new OffLineWidget(this);
    mOffLineWidget->setObjectName(u"mOffLineWidget"_s);
    // After mUsersInRoomFlowWidget
    mRoomWidgetLayout->insertWidget(1, mOffLineWidget);
}

// TODO using it.
void RoomWidget::createOtrWidget()
{
    mOtrWidget = new OtrWidget(this);
    mOtrWidget->setObjectName(u"mOtrWidget"_s);
    connect(mOtrWidget, &OtrWidget::closeOtr, this, &RoomWidget::slotCloseOtr);
    connect(mOtrWidget, &OtrWidget::refreshKeys, this, &RoomWidget::slotRefreshOtrKeys);
    // After mUsersInRoomFlowWidget
    mRoomWidgetLayout->insertWidget(1, mOtrWidget);
}

void RoomWidget::createE2eSaveEncryptionKeyWidget()
{
    if (mCurrentRocketChatAccount && !mCurrentRocketChatAccount->e2eKeyManager()->keySaved()) {
        mE2eSaveEncryptionKeyWidget = new E2eSaveEncryptionKeyWidget(this);
        mE2eSaveEncryptionKeyWidget->setObjectName(u"mE2eDecodeEncryptionKeyWidget"_s);
        connect(mE2eSaveEncryptionKeyWidget, &E2eSaveEncryptionKeyWidget::saveEncrytionKey, this, &RoomWidget::slotGenerateNewPassword);
        // After mUsersInRoomFlowWidget
        mRoomWidgetLayout->insertWidget(1, mE2eSaveEncryptionKeyWidget);
    }
}

// TODO use it
void RoomWidget::createE2eDecodeEncryptionKeyFailedWidget()
{
    mE2eDecodeEncryptionKeyFailedWidget = new E2eDecodeEncryptionKeyFailedWidget(this);
    mE2eDecodeEncryptionKeyFailedWidget->setObjectName(u"mE2eDecodeEncryptionKeyFailedWidget"_s);
    connect(mE2eDecodeEncryptionKeyFailedWidget, &E2eDecodeEncryptionKeyFailedWidget::decodeEncrytionKey, this, &RoomWidget::slotDecodeEncryptionKey);
    // After mUsersInRoomFlowWidget
    mRoomWidgetLayout->insertWidget(1, mE2eDecodeEncryptionKeyFailedWidget);
}

void RoomWidget::slotDecodeEncryptionKey()
{
    QPointer<E2ePasswordDecodeKeyDialog> dlg = new E2ePasswordDecodeKeyDialog(this);
    if (dlg->exec()) {
        // TODO we saved it => don't ask it again
        const QString password = dlg->password();
        // TODO generate private key
    }
    delete dlg;
}

void RoomWidget::createE2eDecodeEncryptionKeyWidget()
{
    mE2eDecodeEncryptionKeyWidget = new E2eDecodeEncryptionKeyWidget(this);
    mE2eDecodeEncryptionKeyWidget->setObjectName(u"mE2eDecodeEncryptionKeyWidget"_s);
    connect(mE2eDecodeEncryptionKeyWidget, &E2eDecodeEncryptionKeyWidget::decodeEncrytionKey, this, &RoomWidget::slotDecodeEncryptionKey);
    // After mUsersInRoomFlowWidget
    mRoomWidgetLayout->insertWidget(1, mE2eDecodeEncryptionKeyWidget);
}

void RoomWidget::slotGenerateNewPassword()
{
    QPointer<E2eCopyPasswordDialog> dlg = new E2eCopyPasswordDialog(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        mCurrentRocketChatAccount->settings()->setKeySaved(true);
        // TODO save it in kwalletmanagers ?
    }
    // Hide it.
    mE2eSaveEncryptionKeyWidget->animatedHide();
    delete dlg;
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
        info.roomId = mRoomWidgetBase->roomId();
        if (KMessageBox::ButtonCode::PrimaryAction
            == KMessageBox::questionTwoActions(this,
                                               i18n("Do you want really remove history?"),
                                               i18nc("@title:window", "Remove History"),
                                               KStandardGuiItem::remove(),
                                               KStandardGuiItem::cancel())) {
            auto job = new RocketChatRestApi::RoomsCleanHistoryJob(this);
            job->setCleanHistoryInfo(info);
            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            if (!job->start()) {
                qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelCleanHistoryJob";
            }
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
        auto job = new RocketChatRestApi::RoomsExportJob(this);
        job->setRoomExportInfo(info);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::RoomsExportJob::roomExportDone, this, &RoomWidget::slotRoomExportDone);
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RoomsExportJob";
        }
    }
    delete dlg;
}

void RoomWidget::slotRoomExportDone()
{
    auto notification = new KNotification(u"export-message"_s, KNotification::CloseOnTimeout);
    notification->setTitle(i18n("Export Messages"));
    notification->setText(i18n("Your email has been queued for sending."));
    notification->sendEvent();
}

void RoomWidget::slotVideoChat()
{
    mCurrentRocketChatAccount->createJitsiConfCall(mRoomWidgetBase->roomId());
}

void RoomWidget::slotAddUsersInRoom()
{
    QPointer<AddUsersInRoomDialog> dlg = new AddUsersInRoomDialog(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const QList<QByteArray> users = dlg->userIds();
        for (const QByteArray &user : users) {
            mCurrentRocketChatAccount->addUserToRoom(user, mRoomWidgetBase->roomId(), mRoomType);
        }
    }
    delete dlg;
}

void RoomWidget::slotInviteUsers()
{
    InviteUsersDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoomId(mRoomWidgetBase->roomId());
    dlg.generateLink();
    dlg.exec();
}

void RoomWidget::displayUiInteractionDialog(const QJsonObject &obj)
{
    auto dialog = new AutoGenerateInteractionUiDialog(mCurrentRocketChatAccount, this);
    if (dialog->parse(obj)) {
        dialog->exec();
        delete dialog;
    } else {
        delete dialog;
    }
}

void RoomWidget::slotShowUiInteraction(const QJsonArray &array)
{
    for (const auto &r : array) {
        Q_EMIT showUiInteractionDialog(r.toObject());
    }
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
    dlg->setFilterProxyModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
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
    dlg->setFilterProxyModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
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
    dlg->setFilterProxyModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
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
    dlg->setFilterProxyModel(mCurrentRocketChatAccount->listMessagesFilterProxyModel());
    const QByteArray currentRoomId{mRoomWidgetBase->roomId()};
    dlg->setRoomId(currentRoomId);
    dlg->setRoom(mRoom);
    mCurrentRocketChatAccount->getListMessages(currentRoomId, ListMessagesModel::ThreadsMessages);
    connect(dlg, &ShowListMessageBaseDialog::goToMessageRequested, this, &RoomWidget::slotGotoMessage);
    dlg->exec();
    delete dlg;
}

void RoomWidget::slotShowDiscussions()
{
    auto dlg = new ShowDiscussionsDialog(mCurrentRocketChatAccount, this);
    dlg->setModel(mCurrentRocketChatAccount->discussionsFilterProxyModel());
    const QByteArray currentRoomId{mRoomWidgetBase->roomId()};
    dlg->setRoomId(currentRoomId);
    mCurrentRocketChatAccount->discussionsInRoom(currentRoomId);
    dlg->show();
}

void RoomWidget::slotShowFileAttachments()
{
    const QByteArray currentRoomId{mRoomWidgetBase->roomId()};
    auto dlg = new ShowAttachmentDialog(mCurrentRocketChatAccount, this);
    mCurrentRocketChatAccount->roomFiles(currentRoomId, mRoomType);
    dlg->setModel(mCurrentRocketChatAccount->filesForRoomFilterProxyModel());
    dlg->setRoomId(currentRoomId);
    dlg->setRoomType(mRoomType);
    dlg->show();
}

void RoomWidget::slotSearchMessages()
{
    if (!mRoom) {
        return;
    }
    if (mRoom->encrypted()) {
        KMessageBox::information(this, i18n("Encrypted content cannot be searched."), i18nc("@title:window", "Search Message"));
        return;
    }
    SearchMessageDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoomId(mRoomWidgetBase->roomId());
    dlg.setRoom(mRoom);
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
        KMessageBox::information(this,
                                 i18n("A workspace admin needs to install and configure a conference call apps."),
                                 i18nc("@title:window", "Video Conference"));
    });
    connect(job, &RocketChatRestApi::VideoConferenceCapabilitiesJob::videoConferenceCapabilitiesDone, this, [this](const QJsonObject &obj) {
        // qDebug() << "obj  " << obj;
        // {"capabilities":{"cam":true,"mic":true,"title":true},"providerName":"jitsi","success":true}
        const QJsonObject capabilitiesObj = obj["capabilities"_L1].toObject();
        const bool useCam = capabilitiesObj["cam"_L1].toBool();
        const bool useMic = capabilitiesObj["mic"_L1].toBool();
        ConferenceCallWidget::ConferenceCallStart callInfo;
        callInfo.useCamera = useCam;
        callInfo.useMic = useMic;

        QPointer<ConferenceCallDialog> dlg = new ConferenceCallDialog(this);
        dlg->setConferenceCallInfo(callInfo);
        if (dlg->exec()) {
            const ConferenceCallWidget::ConferenceCallStart conferenceCallInfo = dlg->conferenceCallInfo();

            auto job = new RocketChatRestApi::VideoConferenceStartJob(this);
            RocketChatRestApi::VideoConferenceStartJob::VideoConferenceStartInfo startInfo;
            startInfo.roomId = mRoomWidgetBase->roomId();
            startInfo.allowRinging = mRoom->hasPermission(u"videoconf-ring-users"_s);
            job->setInfo(startInfo);
            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::VideoConferenceStartJob::videoConferenceStartDone, this, [this, conferenceCallInfo](const QJsonObject &obj) {
                // qDebug() << "obj  " << obj;
                // {"data":{"callId":"63949ea24ef3f3baa9658f25","providerName":"jitsi","rid":"hE6RS3iv5ND5EGWC6","type":"videoconference"},"success":true}
                const QString callId{obj["callId"_L1].toString()};
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
                    RuqolaUtils::self()->openUrl(QUrl(joinObject["url"_L1].toString()));
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
            const AccountRoomSettings::PendingTypedInfo info{
                .text = mRoomWidgetBase->messageLineWidget()->text(),
                .messageIdBeingEdited = mRoomWidgetBase->messageLineWidget()->messageIdBeingEdited(),
                .threadMessageId = mRoomWidgetBase->messageLineWidget()->threadMessageId(),
                .quotePermalink = mRoomWidgetBase->messageLineWidget()->quotePermalink(),
                .quoteText = mRoomWidgetBase->messageLineWidget()->quoteText(),
                .scrollbarPosition = mRoomWidgetBase->messageListView()->verticalScrollBar()->value(),
            };
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
        mRoomWidgetBase->messageLineWidget()->setRoomName(mRoom->displayRoomName());

        mRoomHeaderWidget->setRoomName(mRoom->displayRoomName());
        mRoomHeaderWidget->setRoomAnnouncement(mRoom->displayAnnouncement());
        mRoomHeaderWidget->setRoomTopic(mRoom->displayTopic());
        mRoomHeaderWidget->setFavoriteStatus(mRoom->favorite());
        mRoomHeaderWidget->setEncypted(mRoom->encrypted() && mRoom->hasPermission(u"edit-room"_s));
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
        if (mRoom->channelCounterInfo() && mRoom->channelCounterInfo()->isValid() && mRoom->channelCounterInfo()->unreadMessages() > 0) {
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
            mRoomWidgetBase->messageLineWidget()->setRoomName(mRoom->displayRoomName());
        });
        connect(mRoom, &Room::fnameChanged, this, [this]() {
            mRoomHeaderWidget->setRoomName(mRoom->displayRoomName());
            mRoomWidgetBase->messageLineWidget()->setRoomName(mRoom->displayRoomName());
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
            info.channelType = RocketChatRestApi::ChannelHistoryJob::ChannelType::Channel;
            break;
        case Room::RoomType::Direct:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::ChannelType::Direct;
            break;
        case Room::RoomType::Private:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::ChannelType::Groups;
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
        info.roomId = mRoomWidgetBase->roomId();
        const qint64 endDateTime = roomMessageModel->lastTimestamp();
        info.latestMessage = QDateTime::fromMSecsSinceEpoch(endDateTime, QTimeZone::UTC).toString(Qt::ISODateWithMs);
        // qDebug() << " info.latestMessage " << info.latestMessage;
        job->setChannelHistoryInfo(info);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::ChannelHistoryJob::channelHistoryDone, this, [this, numberOfMessage, roomMessageModel](const QJsonObject &obj) {
            mCurrentRocketChatAccount->rocketChatBackend()->processIncomingMessages(obj.value("messages"_L1).toArray(), true, true);
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
            info.channelType = RocketChatRestApi::ChannelHistoryJob::ChannelType::Channel;
            break;
        case Room::RoomType::Direct:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::ChannelType::Direct;
            break;
        case Room::RoomType::Private:
            info.channelType = RocketChatRestApi::ChannelHistoryJob::ChannelType::Groups;
            break;
        case Room::RoomType::Unknown:
            qCWarning(RUQOLAWIDGETS_LOG) << " Problem with room type ";
            break;
        }
        if (mRoomType == Room::RoomType::Unknown) {
            return;
        }
        auto job = new RocketChatRestApi::ChannelHistoryJob(this);
        info.roomId = mRoomWidgetBase->roomId();
        const qint64 endDateTime = messageModel->lastTimestamp();
        info.latestMessage = QDateTime::fromMSecsSinceEpoch(endDateTime, QTimeZone::UTC).toString(Qt::ISODateWithMs);
        info.oldestMessage = messageDateTimeUtc;
        info.inclusive = true;
        info.count = 5000;
        // qDebug() << " info " << info;
        job->setChannelHistoryInfo(info);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::ChannelHistoryJob::channelHistoryDone, this, [messageId, messageModel, messageListView, this](const QJsonObject &obj) {
            mCurrentRocketChatAccount->rocketChatBackend()->processIncomingMessages(obj.value("messages"_L1).toArray(), true, true);
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
    info.roomId = mRoomWidgetBase->roomId();
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
#if USE_E2E_SUPPORT
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::needToSaveE2EPassword, this, &RoomWidget::createE2eSaveEncryptionKeyWidget);
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::needToDecryptE2EPassword, this, &RoomWidget::createE2eDecodeEncryptionKeyWidget);
#endif
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::showUiInteraction, this, &RoomWidget::slotShowUiInteraction);
    }
    mCurrentRocketChatAccount = account;
    mRoomWidgetBase->setCurrentRocketChatAccount(account);

    connect(mCurrentRocketChatAccount, &RocketChatAccount::openThreadRequested, this, &RoomWidget::slotOpenThreadRequested);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::displayReconnectWidget, this, &RoomWidget::slotDisplayReconnectWidget);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RoomWidget::slotLoginStatusChanged);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::needUpdateMessageView, this, &RoomWidget::updateListView);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::showUiInteraction, this, &RoomWidget::slotShowUiInteraction);

#if USE_E2E_SUPPORT
    auto showE2eDecodeEncryptionKeyWidget = [this] {
        if (!mE2eDecodeEncryptionKeyWidget) {
            createE2eDecodeEncryptionKeyWidget();
        }
        mE2eDecodeEncryptionKeyWidget->animatedShow();
    };

    auto showE2eSaveEncryptionKeyWidget = [this] {
        if (mCurrentRocketChatAccount && !mCurrentRocketChatAccount->e2eKeyManager()->keySaved()) {
            if (!mE2eSaveEncryptionKeyWidget) {
                createE2eSaveEncryptionKeyWidget();
            }
            mE2eSaveEncryptionKeyWidget->animatedShow();
        }
    };

    connect(mCurrentRocketChatAccount, &RocketChatAccount::needToSaveE2EPassword, this, [showE2eSaveEncryptionKeyWidget]() {
        showE2eSaveEncryptionKeyWidget();
    });
    connect(mCurrentRocketChatAccount, &RocketChatAccount::needToDecryptE2EPassword, this, [showE2eDecodeEncryptionKeyWidget]() {
        showE2eDecodeEncryptionKeyWidget();
    });
    // Hide them
    if (mE2eSaveEncryptionKeyWidget && !mCurrentRocketChatAccount->e2EPasswordMustBeSave()) {
        mE2eSaveEncryptionKeyWidget->animatedHide();
    } else if (mCurrentRocketChatAccount->e2EPasswordMustBeSave()) {
        showE2eSaveEncryptionKeyWidget();
    }

    if (mE2eDecodeEncryptionKeyWidget && !mCurrentRocketChatAccount->e2EPasswordMustBeDecrypt()) {
        mE2eDecodeEncryptionKeyWidget->animatedHide();
    } else if (mCurrentRocketChatAccount->e2EPasswordMustBeDecrypt()) {
        showE2eDecodeEncryptionKeyWidget();
    }
#endif

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
    // Don't show all the time. => show only if after 5 secondes
    if (seconds > 5) {
        // Disable for the moment it seems to create some problems
        if (!mRoomReconnectInfoWidget) {
            createRoomReconnectInfoWidget();
        }
        mRoomReconnectInfoWidget->setReconnectSecondDelay(seconds);
    }
}

void RoomWidget::slotCloseOtr()
{
    mCurrentRocketChatAccount->streamNotifyUserOtrEnd(roomId(), mCurrentRocketChatAccount->userId());
}

void RoomWidget::slotRefreshOtrKeys()
{
    // TODO
}

#include "moc_roomwidget.cpp"
