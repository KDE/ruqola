/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccount.h"
#include "attachments/fileattachments.h"
#include "authenticationmanager.h"
#include "autotranslate/getsupportedlanguagesjob.h"
#include "commands/listcommandsjob.h"
#include "config-ruqola.h"
#include "customemojiiconmanager.h"
#include "downloadappslanguages/downloadappslanguagesmanager.h"
#include "emoticons/emojimanager.h"
#include "managerdatapaths.h"
#include "messagequeue.h"
#include "rooms/roomsexportjob.h"

#include "model/autotranslatelanguagesmodel.h"
#include "model/commandsmodel.h"
#include "model/commonmessagefilterproxymodel.h"
#include "model/commonmessagesmodel.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/discussionsmodel.h"
#include "model/emoticonmodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include "model/listmessagesfilterproxymodel.h"
#include "model/loginmethodmodel.h"
#include "model/messagesmodel.h"
#include "model/searchchannelmodel.h"
#include "model/statusmodel.h"
#include "model/threadmessagemodel.h"
#include "model/usercompleterfilterproxymodel.h"
#include "model/usercompletermodel.h"
#include "model/usersforroommodel.h"
#include "model/usersmodel.h"
#include "otr/otrmanager.h"
#include "rocketchatbackend.h"
#include "rocketchatcache.h"
#include "ruqola.h"
#include "ruqola_debug.h"
#include "ruqola_notification_debug.h"
#include "ruqola_reconnect_core_debug.h"
#include "ruqola_typing_notification_debug.h"
#include "ruqolaglobalconfig.h"
#include "ruqolalogger.h"
#include "typingnotification.h"
#include <TextEmoticonsCore/UnicodeEmoticonManager>

#include "channelcounterinfo.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "directmessage/opendmjob.h"
#include "discussions/discussions.h"
#include "emoji/loademojicustomjob.h"
#include "license/licensesisenterprisejob.h"
#include "listmessages.h"
#include "localdatabase/localdatabasemanager.h"
#include "localdatabase/localdatabaseutils.h"
#include "managechannels.h"
#include "managelocaldatabase.h"
#include "messagecache.h"
#include "misc/roleslistjob.h"
#include "receivetypingnotificationmanager.h"
#include "ruqola_thread_message_debug.h"
#include "uploadfilemanager.h"
#include "videoconference/videoconferencemanager.h"
#include "videoconference/videoconferencemessageinfomanager.h"

#include <KLocalizedString>
#include <KNotification>
#include <QDesktopServices>
#include <QJsonArray>
#include <QTimer>
#include <TextEmoticonsCore/EmojiModel>
#include <TextEmoticonsCore/EmojiModelManager>
#include <channelgroupbasejob.h>

#if HAVE_NETWORKMANAGER
#include <NetworkManagerQt/Manager>
#endif

#if HAVE_SOLID
#include <Solid/Power>
#endif

#include <plugins/pluginauthentication.h>
#include <plugins/pluginauthenticationinterface.h>

#include "away/awaymanager.h"
#include "customsound/customsoundsmanager.h"
#include "model/switchchannelhistorymodel.h"
#include "users/setstatusjob.h"
#include "users/usersautocompletejob.h"

RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)
    : QObject(parent)
    , mAccountRoomSettings(new AccountRoomSettings)
    , mUserModel(new UsersModel(this))
    , mRoomModel(new RoomModel(this, this))
    , mRuqolaServerConfig(new RuqolaServerConfig)
    , mUserCompleterModel(new UserCompleterModel(this))
    , mStatusModel(new StatusModel(this))
    , mOtrManager(new OtrManager(this, this))
    , mSearchChannelModel(new SearchChannelModel(this))
    , mLoginMethodModel(new LoginMethodModel(this))
    , mInputTextManager(new InputTextManager(this, this))
    , mInputThreadMessageTextManager(new InputTextManager(this, this))
    , mReceiveTypingNotificationManager(new ReceiveTypingNotificationManager(this))
    , mDiscussionsModel(new DiscussionsModel(this))
    , mCommandsModel(new CommandsModel(this))
    , mAutoTranslateLanguagesModel(new AutotranslateLanguagesModel(this))
    , mDownloadAppsLanguagesManager(new DownloadAppsLanguagesManager(this))
    , mMessageCache(new MessageCache(this, this))
    , mManageChannels(new ManageChannels(this, this))
    , mCustomSoundManager(new CustomSoundsManager(this))
    , mAwayManager(new AwayManager(this, this))
    , mSwitchChannelHistoryModel(new SwitchChannelHistoryModel(this))
    , mUploadFileManager(new UploadFileManager(this, this))
    , mVideoConferenceManager(new VideoConferenceManager(this, this))
    , mVideoConferenceMessageInfoManager(new VideoConferenceMessageInfoManager(this, this))
    , mLocalDatabaseManager(std::make_unique<LocalDatabaseManager>())
    , mManageLoadHistory(new ManageLocalDatabase(this, this))
{
    qCDebug(RUQOLA_LOG) << " RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)" << accountFileName;
    // create an unique file for each account
    loadSettings(accountFileName);
#if 0 // Disable it  otherwise autotests failed
    if (!mSettings->isValid()) {
        return;
    }
#endif
    if (!qEnvironmentVariableIsEmpty("RUQOLA_LOGFILE")) {
        mRuqolaLogger = new RuqolaLogger(mSettings->accountName());
    }

    mServerConfigInfo = new ServerConfigInfo(this, this);
    // Create it before loading settings

    mInputTextManager->setObjectName(QStringLiteral("mInputTextManager"));
    connect(mInputTextManager,
            &InputTextManager::completionRequested,
            this,
            [this](const QString &roomId, const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type) {
                inputAutocomplete(roomId, pattern, exceptions, type, false);
            });

    mInputThreadMessageTextManager->setObjectName(QStringLiteral("mInputThreadMessageTextManager"));
    connect(mInputThreadMessageTextManager,
            &InputTextManager::completionRequested,
            this,
            [this](const QString &roomId, const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type) {
                inputAutocomplete(roomId, pattern, exceptions, type, true);
            });

    initializeAuthenticationPlugins();

    mRocketChatBackend = new RocketChatBackend(this, this);

    // Load list of unicode emoticon
    TextEmoticonsCore::UnicodeEmoticonManager::self();

    // After loadSettings
    mEmojiManager = new EmojiManager(this, this);
    mEmojiManager->setServerUrl(mSettings->serverUrl());
    connect(mEmojiManager, &EmojiManager::customEmojiChanged, this, &RocketChatAccount::updateCustomEmojiList);

    mEmoticonModel = new EmoticonModel(this, this);
    mEmoticonModel->setUnicodeEmoticons(mEmojiManager->unicodeEmojiList());
    mInputTextManager->setEmoticonModel(mEmoticonModel);
    mInputThreadMessageTextManager->setEmoticonModel(mEmoticonModel);

    mUserCompleterFilterModelProxy = new UserCompleterFilterProxyModel(this);
    mUserCompleterFilterModelProxy->setSourceModel(mUserCompleterModel);

    mSearchMessageModel = new CommonMessagesModel(this, this);
    mSearchMessageFilterProxyModel = new CommonMessageFilterProxyModel(mSearchMessageModel, this);

    mFilesModelForRoom = new FilesForRoomModel(this, this);
    mFilesModelForRoom->setObjectName(QStringLiteral("filesmodelforrooms"));
    mFilesForRoomFilterProxyModel = new FilesForRoomFilterProxyModel(mFilesModelForRoom, this);
    mFilesForRoomFilterProxyModel->setObjectName(QStringLiteral("filesforroomfiltermodelproxy"));

    mDiscussionsModel->setObjectName(QStringLiteral("discussionsmodel"));
    mDiscussionsFilterProxyModel = new DiscussionsFilterProxyModel(mDiscussionsModel, this);
    mDiscussionsFilterProxyModel->setObjectName(QStringLiteral("discussionsfilterproxymodel"));

    mThreadMessageModel = new ThreadMessageModel(this, this);
    mThreadMessageModel->setObjectName(QStringLiteral("threadmessagemodel"));

    mListMessageModel = new ListMessagesModel(QString(), this, nullptr, this);
    mListMessageModel->setObjectName(QStringLiteral("listmessagemodel"));

    mListMessagesFilterProxyModel = new ListMessagesFilterProxyModel(mListMessageModel, this);
    mListMessagesFilterProxyModel->setObjectName(QStringLiteral("listmessagesfiltermodelproxy"));

    mAutoTranslateLanguagesModel->setObjectName(QStringLiteral("autotranslatelanguagesmodel"));

    connect(mRoomModel, &RoomModel::needToUpdateNotification, this, &RocketChatAccount::slotNeedToUpdateNotification);
    connect(mRoomModel, &RoomModel::roomNeedAttention, this, &RocketChatAccount::slotRoomNeedAttention);
    connect(mRoomModel, &RoomModel::roomRemoved, this, &RocketChatAccount::roomRemoved);

    mMessageQueue = new MessageQueue(this, this);
    mTypingNotification = new TypingNotification(this);
    mCache = new RocketChatCache(this, this);

    connect(mDownloadAppsLanguagesManager, &DownloadAppsLanguagesManager::fileLanguagesParseSuccess, this, &RocketChatAccount::slotFileLanguagedParsed);
    connect(mDownloadAppsLanguagesManager, &DownloadAppsLanguagesManager::fileLanguagesParseFailed, this, &RocketChatAccount::slotFileLanguagedParsed);

    connect(mCache, &RocketChatCache::fileDownloaded, this, &RocketChatAccount::fileDownloaded);
    connect(mTypingNotification, &TypingNotification::informTypingStatus, this, &RocketChatAccount::slotInformTypingStatus);
    connect(this, &RocketChatAccount::customUserStatusChanged, this, &RocketChatAccount::slotUpdateCustomUserStatus);
    QTimer::singleShot(0, this, &RocketChatAccount::clearModels);

#if HAVE_SOLID
    connect(Solid::Power::self(), &Solid::Power::resumeFromSuspend, this, &RocketChatAccount::slotReconnectToServer);
#endif

#if HAVE_NETWORKMANAGER
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::primaryConnectionChanged, this, [=](const QString &uni) {
        // If there is a new network connection, log out and back. The uni is "/" when the last primary connection
        // was closed. Do not log out to keep the messages visible. Login only if we were logged in at this point.
        if (uni != QLatin1String("/") && mDdp) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "Reconnect and logout : " << accountName();
            logOut();
            slotReconnectToServer();
        }
    });
#endif
    connect(mManageChannels, &ManageChannels::selectRoomByRoomIdRequested, this, &RocketChatAccount::selectRoomByRoomIdRequested);
    connect(mManageChannels, &ManageChannels::selectRoomByRoomNameRequested, this, &RocketChatAccount::selectRoomByRoomNameRequested);
    connect(mManageChannels, &ManageChannels::missingChannelPassword, this, &RocketChatAccount::missingChannelPassword);
    connect(mManageChannels, &ManageChannels::openArchivedRoom, this, &RocketChatAccount::openArchivedRoom);
    connect(&mRolesManager, &RolesManager::rolesChanged, this, &RocketChatAccount::rolesUpdated);
    connect(mCustomSoundManager, &CustomSoundsManager::customSoundRemoved, this, &RocketChatAccount::customSoundRemoved);
    connect(mCustomSoundManager, &CustomSoundsManager::customSoundAdded, this, &RocketChatAccount::customSoundAdded);
    connect(mCustomSoundManager, &CustomSoundsManager::customSoundUpdated, this, &RocketChatAccount::customSoundUpdated);
    connect(mAwayManager, &AwayManager::awayChanged, this, &RocketChatAccount::slotAwayStatusChanged);
}

RocketChatAccount::~RocketChatAccount()
{
    delete mCache;
    mCache = nullptr;

    delete mRuqolaServerConfig;
    delete mRuqolaLogger;
    delete mAccountRoomSettings;
}

void RocketChatAccount::reconnectToServer()
{
    slotReconnectToServer();
}

Room::TeamRoomInfo RocketChatAccount::roomFromTeamId(const QString &teamId) const
{
    return mRoomModel->roomFromTeamId(teamId);
}

void RocketChatAccount::removeSettings()
{
    mSettings->removeSettings();
}

void RocketChatAccount::loadSettings(const QString &accountFileName)
{
    delete mSettings;
    mSettings = new RocketChatAccountSettings(accountFileName, this);
    if (mSettings->isValid()) {
        connect(mSettings, &RocketChatAccountSettings::serverURLChanged, this, &RocketChatAccount::serverUrlChanged);
        connect(mSettings, &RocketChatAccountSettings::userIdChanged, this, &RocketChatAccount::userIdChanged);
        connect(mSettings, &RocketChatAccountSettings::userNameChanged, this, &RocketChatAccount::userNameChanged);
        connect(mSettings, &RocketChatAccountSettings::passwordChanged, this, &RocketChatAccount::passwordChanged);
    }
}

void RocketChatAccount::slotRoomNeedAttention()
{
    qCDebug(RUQOLA_NOTIFICATION_LOG) << " emit alert"
                                     << " account name: " << accountName();
    Q_EMIT roomNeedAttention();
}

void RocketChatAccount::setOwnUserPreferences(const OwnUserPreferences &ownUserPreferences)
{
    mOwnUser.setOwnUserPreferences(ownUserPreferences);
}

OwnUser RocketChatAccount::ownUser() const
{
    return mOwnUser;
}

void RocketChatAccount::cleanChannelHistory(const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &info)
{
    restApi()->cleanChannelHistory(info);
}

void RocketChatAccount::slotNeedToUpdateNotification()
{
    bool hasAlert = false;
    int nbUnread = 0;
    mRoomModel->getUnreadAlertFromAccount(hasAlert, nbUnread);
    Q_EMIT updateNotification(hasAlert, nbUnread, accountName());
}

void RocketChatAccount::clearModels()
{
    // Clear rooms data and refill it with data in the cache, if there is
    mRoomModel->reset();

    mMessageQueue->loadCache();
    // Try to send queue message
    mMessageQueue->processQueue();
}

SearchChannelModel *RocketChatAccount::searchChannelModel() const
{
    return mSearchChannelModel;
}

UserCompleterModel *RocketChatAccount::userCompleterModel() const
{
    return mUserCompleterModel;
}

UserCompleterFilterProxyModel *RocketChatAccount::userCompleterFilterModelProxy() const
{
    return mUserCompleterFilterModelProxy;
}

EmojiManager *RocketChatAccount::emojiManager() const
{
    return mEmojiManager;
}

QString RocketChatAccount::userStatusStr(const QString &name)
{
    return mUserModel->userStatusStr(name);
}

QString RocketChatAccount::userStatusIconFileName(const QString &name)
{
    return mUserModel->userStatusIconFileName(name);
}

bool RocketChatAccount::userIsOffline(const QString &name)
{
    return mUserModel->userIsOffline(name);
}

StatusModel *RocketChatAccount::statusModel() const
{
    return mStatusModel;
}

RuqolaServerConfig *RocketChatAccount::ruqolaServerConfig() const
{
    return mRuqolaServerConfig;
}

RuqolaLogger *RocketChatAccount::ruqolaLogger() const
{
    return mRuqolaLogger;
}

UsersForRoomModel *RocketChatAccount::usersModelForRoom(const QString &roomId) const
{
    return mRoomModel->usersModelForRoom(roomId);
}

FilesForRoomFilterProxyModel *RocketChatAccount::filesForRoomFilterProxyModel() const
{
    return mFilesForRoomFilterProxyModel;
}

RocketChatBackend *RocketChatAccount::rocketChatBackend() const
{
    return mRocketChatBackend;
}

MessageQueue *RocketChatAccount::messageQueue() const
{
    return mMessageQueue;
}

RocketChatAccountSettings *RocketChatAccount::settings() const
{
    return mSettings;
}

void RocketChatAccount::slotInformTypingStatus(const QString &room, bool typing)
{
    qCDebug(RUQOLA_TYPING_NOTIFICATION_LOG) << " slotInformTypingStatus room " << room << " typing " << typing;
    ddp()->informTypingStatus(room, typing, mSettings->userName());
}

RoomModel *RocketChatAccount::roomModel() const
{
    return mRoomModel;
}

UsersModel *RocketChatAccount::usersModel() const
{
    return mUserModel;
}

Room *RocketChatAccount::room(const QString &roomId)
{
    return mRoomModel->findRoom(roomId);
}

DiscussionsFilterProxyModel *RocketChatAccount::discussionsFilterProxyModel() const
{
    return mDiscussionsFilterProxyModel;
}

MessagesModel *RocketChatAccount::messageModelForRoom(const QString &roomID)
{
    return mRoomModel->messageModel(roomID);
}

void RocketChatAccount::changeShowOriginalMessage(const QString &roomId, const QString &messageId, bool showOriginal)
{
    MessagesModel *model = mRoomModel->messageModel(roomId);
    if (model) {
        model->changeShowOriginalMessage(messageId, showOriginal);
    } else {
        qCWarning(RUQOLA_LOG) << "impossible to find room: " << roomId;
    }
}

QString RocketChatAccount::getUserCurrentMessage(const QString &roomId)
{
    return mRoomModel->inputMessage(roomId);
}

void RocketChatAccount::setUserCurrentMessage(const QString &message, const QString &roomId)
{
    mRoomModel->setInputMessage(roomId, message);
}

void RocketChatAccount::textEditing(const QString &roomId, bool clearNotification)
{
    mTypingNotification->textNotificationChanged(roomId, clearNotification);
}

void RocketChatAccount::reactOnMessage(const QString &messageId, const QString &emoji, bool shouldReact)
{
    if (emoji.startsWith(QLatin1Char(':')) && emoji.endsWith(QLatin1Char(':'))) {
        restApi()->reactOnMessage(messageId, emoji, shouldReact);
    } else {
        restApi()->reactOnMessage(messageId, mEmojiManager->normalizedReactionEmoji(emoji), shouldReact);
    }
}

void RocketChatAccount::sendMessage(const QString &roomID, const QString &message)
{
    restApi()->postMessage(roomID, message);
    markRoomAsRead(roomID);
}

void RocketChatAccount::updateMessage(const QString &roomID, const QString &messageId, const QString &message)
{
    restApi()->updateMessage(roomID, messageId, message);
}

void RocketChatAccount::replyOnThread(const QString &roomID, const QString &threadMessageId, const QString &message)
{
    restApi()->sendMessage(roomID, message, QString(), threadMessageId);
}

void RocketChatAccount::deleteFileMessage(const QString &roomId, const QString &fileId, Room::RoomType channelType)
{
    ddp()->deleteFileMessage(roomId, fileId, channelType);
}

QString RocketChatAccount::avatarUrl(const Utils::AvatarInfo &info)
{
    return mCache->avatarUrl(info);
}

void RocketChatAccount::insertAvatarUrl(const QString &userId, const QUrl &url)
{
    mCache->insertAvatarUrl(userId, url);
}

RocketChatRestApi::Connection *RocketChatAccount::restApi()
{
    if (!mRestApi) {
        mRestApi = new RocketChatRestApi::Connection(this);
        connect(mRestApi, &RocketChatRestApi::Connection::channelMembersDone, this, &RocketChatAccount::parseUsersForRooms);
        connect(mRestApi, &RocketChatRestApi::Connection::channelFilesDone, this, &RocketChatAccount::slotChannelFilesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::channelRolesDone, this, &RocketChatAccount::slotChannelGroupRolesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::groupRolesDone, this, &RocketChatAccount::slotChannelGroupRolesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::searchMessageDone, this, &RocketChatAccount::slotSearchMessages);
        connect(mRestApi, &RocketChatRestApi::Connection::failed, this, &RocketChatAccount::slotJobFailed);
        connect(mRestApi, &RocketChatRestApi::Connection::spotlightDone, this, &RocketChatAccount::slotSplotLightDone);
        connect(mRestApi, &RocketChatRestApi::Connection::directoryDone, this, &RocketChatAccount::slotDirectoryDone);
        connect(mRestApi, &RocketChatRestApi::Connection::getThreadMessagesDone, this, &RocketChatAccount::slotGetThreadMessagesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::getDiscussionsDone, this, &RocketChatAccount::slotGetDiscussionsListDone);
        connect(mRestApi, &RocketChatRestApi::Connection::channelListDone, this, &RocketChatAccount::slotChannelListDone);
        connect(mRestApi, &RocketChatRestApi::Connection::markAsReadDone, this, &RocketChatAccount::slotMarkAsReadDone);
        connect(mRestApi, &RocketChatRestApi::Connection::postMessageDone, this, &RocketChatAccount::slotPostMessageDone);
        connect(mRestApi, &RocketChatRestApi::Connection::updateMessageFailed, this, &RocketChatAccount::updateMessageFailed);

        connect(mRestApi, &RocketChatRestApi::Connection::getThreadsDone, this, [this](const QJsonObject &obj, const QString &roomId, bool onlyUnread) {
            slotGetListMessagesDone(obj,
                                    roomId,
                                    onlyUnread ? ListMessagesModel::ListMessageType::UnreadThreadsMessages
                                               : ListMessagesModel::ListMessageType::ThreadsMessages);
        });
        connect(mRestApi, &RocketChatRestApi::Connection::getMentionedMessagesDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::MentionsMessages);
        });
        connect(mRestApi, &RocketChatRestApi::Connection::getPinnedMessagesDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::PinnedMessages);
        });
        connect(mRestApi, &RocketChatRestApi::Connection::getStarredMessagesDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::StarredMessages);
        });

        connect(mRestApi, &RocketChatRestApi::Connection::usersPresenceDone, this, &RocketChatAccount::slotUsersPresenceDone);
        connect(mRestApi, &RocketChatRestApi::Connection::usersAutocompleteDone, this, &RocketChatAccount::slotUserAutoCompleterDone);
        connect(mRestApi, &RocketChatRestApi::Connection::registerUserDone, this, &RocketChatAccount::slotRegisterUserDone);
        connect(mRestApi, &RocketChatRestApi::Connection::channelGetCountersDone, this, &RocketChatAccount::slotChannelGetCountersDone);
        connect(mRestApi, &RocketChatRestApi::Connection::customUserStatusDone, this, &RocketChatAccount::slotCustomUserStatusDone);
        connect(mRestApi, &RocketChatRestApi::Connection::permissionListAllDone, this, &RocketChatAccount::slotPermissionListAllDone);
        connect(mRestApi, &RocketChatRestApi::Connection::usersSetPreferencesDone, this, &RocketChatAccount::slotUsersSetPreferencesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::networkSessionFailedError, this, [this]() {
            qCDebug(RUQOLA_RECONNECT_LOG) << "networkSessionFailedError Reconnect and logout : " << accountName();
            logOut();
            slotReconnectToServer();
        });

        mRestApi->setServerUrl(mSettings->serverUrl());
        mRestApi->setRestApiLogger(mRuqolaLogger);
        mCache->setRestApiConnection(mRestApi);
    }
    return mRestApi;
}

void RocketChatAccount::slotJobFailed(const QString &str)
{
    Q_EMIT jobFailed(str, accountName());
}

const BannerInfos &RocketChatAccount::bannerInfos() const
{
    return mBannerInfos;
}

UploadFileManager *RocketChatAccount::uploadFileManager() const
{
    return mUploadFileManager;
}

int RocketChatAccount::messageMaximumAllowedSize() const
{
    return mRuqolaServerConfig->messageMaximumAllowedSize();
}

bool RocketChatAccount::messageAllowConvertLongMessagesToAttachment() const
{
    return mRuqolaServerConfig->messageAllowConvertLongMessagesToAttachment();
}

bool RocketChatAccount::useRealName() const
{
    return mRuqolaServerConfig->useRealName();
}

SwitchChannelHistoryModel *RocketChatAccount::switchChannelHistoryModel() const
{
    return mSwitchChannelHistoryModel;
}

const QStringList &RocketChatAccount::searchListCompletion() const
{
    return mSearchListCompletion;
}

void RocketChatAccount::setSearchListCompletion(const QStringList &newSearchListCompletion)
{
    mSearchListCompletion = newSearchListCompletion;
}

void RocketChatAccount::leaveRoom(const QString &identifier, Room::RoomType channelType)
{
    switch (channelType) {
    case Room::RoomType::Private:
        restApi()->leaveGroups(identifier);
        break;
    case Room::RoomType::Channel:
        restApi()->leaveChannel(identifier);
        break;
    case Room::RoomType::Direct:
    case Room::RoomType::Unknown:
        qCWarning(RUQOLA_LOG) << " unsupported leave room for type " << channelType;
        break;
    }
}

void RocketChatAccount::hideRoom(const QString &roomId, Room::RoomType channelType)
{
    restApi()->closeChannel(roomId, Room::roomFromRoomType(channelType));
}

DDPClient *RocketChatAccount::ddp()
{
    if (!mDdp) {
        mDdp = new DDPClient(this, this);
        connect(mDdp->authenticationManager(), &DDPAuthenticationManager::loginStatusChanged, this, &RocketChatAccount::loginStatusChangedSlot);
        connect(mDdp, &DDPClient::connectedChanged, this, &RocketChatAccount::connectedChanged);
        connect(mDdp, &DDPClient::changed, this, &RocketChatAccount::changed);
        connect(mDdp, &DDPClient::added, this, &RocketChatAccount::added);
        connect(mDdp, &DDPClient::removed, this, &RocketChatAccount::removed);
        connect(mDdp, &DDPClient::socketError, this, &RocketChatAccount::socketError);
        connect(mDdp, &DDPClient::disconnectedByServer, this, &RocketChatAccount::slotReconnectToServer);

        if (mSettings) {
            mDdp->setServerUrl(mSettings->serverUrl());
        }
        mDdp->start();
    }
    return mDdp;
}

bool RocketChatAccount::editingMode() const
{
    return mEditingMode;
}

DDPAuthenticationManager::LoginStatus RocketChatAccount::loginStatus()
{
    // TODO: DDP API should exist as soon as the hostname is known
    if (mDdp) {
        return ddp()->authenticationManager()->loginStatus();
    } else {
        return DDPAuthenticationManager::LoggedOut;
    }
}

void RocketChatAccount::tryLogin()
{
    qCDebug(RUQOLA_LOG) << "Attempting login" << mSettings->userName() << "on" << mSettings->serverUrl();

    // ddp() creates a new DDPClient object if it doesn't exist.
    ddp()->enqueueLogin();

    // In the meantime, load cache...
    mRoomModel->reset();
}

void RocketChatAccount::logOut()
{
    mSettings->logout();
    mRoomModel->clear();
    if (mDdp) {
        mDdp->authenticationManager()->logout();
        delete mDdp;
        mDdp = nullptr;
    }
    delete mRestApi;
    mRestApi = nullptr;
}

void RocketChatAccount::clearAllUnreadMessages()
{
    for (int roomIdx = 0, nRooms = mRoomModel->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = mRoomModel->index(roomIdx);
        const auto roomId = roomModelIndex.data(RoomModel::RoomId).toString();
        const bool roomHasAlert = roomModelIndex.data(RoomModel::RoomAlert).toBool();
        if (roomHasAlert) {
            markRoomAsRead(roomId);
        }
    }
}

void RocketChatAccount::markRoomAsRead(const QString &roomId)
{
    mMarkUnreadThreadsAsReadOnNextReply = true;
    restApi()->markRoomAsRead(roomId);
    if (threadsEnabled()) {
        getListMessages(roomId, ListMessagesModel::UnreadThreadsMessages);
    }
}

void RocketChatAccount::changeFavorite(const QString &roomId, bool checked)
{
    restApi()->markAsFavorite(roomId, checked);
}

void RocketChatAccount::openPrivateGroup(const QString &roomId, ChannelTypeInfo typeInfo)
{
    mManageChannels->openPrivateGroup(roomId, typeInfo);
}

void RocketChatAccount::openChannel(const QString &roomId, ChannelTypeInfo typeInfo)
{
    mManageChannels->openChannel(roomId, typeInfo);
}

void RocketChatAccount::openArchivedRoom(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    // TODO
}

void RocketChatAccount::joinJitsiConfCall(const QString &roomId)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::joinJitsiConfCall(const QString &roomId)" << roomId;
    // const QString hash = QString::fromLatin1(QCryptographicHash::hash((mRuqolaServerConfig->uniqueId() + roomId).toUtf8(), QCryptographicHash::Md5).toHex());
    const QString hash = mRuqolaServerConfig->uniqueId() + roomId;
#if defined(Q_OS_IOS)
    const QString scheme = QStringLiteral("org.jitsi.meet://");
#else
    const QString scheme = QStringLiteral("https://");
#endif
    const QString url = scheme + mRuqolaServerConfig->jitsiMeetUrl() + QLatin1Char('/') + mRuqolaServerConfig->jitsiMeetPrefix() + hash;
    const QUrl clickedUrl = QUrl::fromUserInput(url);
    QDesktopServices::openUrl(clickedUrl);
}

void RocketChatAccount::eraseRoom(const QString &roomId, Room::RoomType channelType)
{
    switch (channelType) {
    case Room::RoomType::Private:
        restApi()->groupDelete(roomId);
        break;
    case Room::RoomType::Channel:
        restApi()->channelDelete(roomId);
        break;
    case Room::RoomType::Direct:
    case Room::RoomType::Unknown:
        qCWarning(RUQOLA_LOG) << " unsupported delete for type " << channelType;
        break;
    }
}

void RocketChatAccount::openDirectChannel(const QString &roomId)
{
    if (hasPermission(QStringLiteral("create-d"))) {
        auto job = new RocketChatRestApi::OpenDmJob(this);
        job->setDirectUserId(roomId);
        restApi()->initializeRestApiJob(job);
        // TODO ????
        // connect(job, &RocketChatRestApi::OpenDmJob::openDmDone, this, &RolesManager::parseRoles);
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start OpenDmJob job";
        }
        qDebug() << "Open direct conversation channel with" << roomId;
    }
}

void RocketChatAccount::createNewChannel(const RocketChatRestApi::CreateChannelTeamInfo &info)
{
    if (!info.name.trimmed().isEmpty()) {
        if (info.privateChannel) {
            restApi()->createGroups(info);
        } else {
            restApi()->createChannels(info);
        }
    } else {
        qCDebug(RUQOLA_LOG) << "Channel name can't be empty";
    }
}

void RocketChatAccount::joinDiscussion(const QString &roomId, const QString &joinCode)
{
    ddp()->joinRoom(roomId, joinCode);
}

void RocketChatAccount::joinRoom(const QString &roomId, const QString &joinCode)
{
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    mManageChannels->channelJoin(info, joinCode);
}

void RocketChatAccount::channelAndPrivateAutocomplete(const QString &pattern)
{
    if (pattern.isEmpty()) {
        searchChannelModel()->clear();
    } else {
        // Use restapi
        // Avoid to show own user
        // restApi()->searchRooms(pattern);
        restApi()->searchRoomUser(pattern);
    }
}

void RocketChatAccount::listEmojiCustom()
{
    auto job = new RocketChatRestApi::LoadEmojiCustomJob(this);
    restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::LoadEmojiCustomJob::loadEmojiCustomDone, this, &RocketChatAccount::loadEmoji);
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start listEmojiCustom job";
    }
}

void RocketChatAccount::setDefaultStatus(User::PresenceStatus status, const QString &messageStatus)
{
    RocketChatRestApi::SetStatusJob::StatusType type = RocketChatRestApi::SetStatusJob::Unknown;
    switch (status) {
    case User::PresenceStatus::PresenceOnline:
        type = RocketChatRestApi::SetStatusJob::OnLine;
        break;
    case User::PresenceStatus::PresenceBusy:
        type = RocketChatRestApi::SetStatusJob::Busy;
        break;
    case User::PresenceStatus::PresenceAway:
        type = RocketChatRestApi::SetStatusJob::Away;
        break;
    case User::PresenceStatus::PresenceOffline:
        type = RocketChatRestApi::SetStatusJob::Offline;
        break;
    case User::PresenceStatus::Unknown:
        type = RocketChatRestApi::SetStatusJob::Unknown;
        break;
    }
    mPresenceStatus = status;
    // qDebug() << "RocketChatAccount::setDefaultStatus  " << messageStatus;
    restApi()->setUserStatus(userId(), type, messageStatus);
}

QList<TextEmoticonsCore::CustomEmoji> RocketChatAccount::customEmojies() const
{
    QList<TextEmoticonsCore::CustomEmoji> mCustomEmojies;
    const auto customEmojiList = mEmojiManager->customEmojiList();
    for (const auto &emoji : customEmojiList) {
        TextEmoticonsCore::CustomEmoji custom;
        custom.setIdentifier(emoji.emojiIdentifier());
        custom.setIsAnimatedEmoji(emoji.isAnimatedImage());
        mCustomEmojies.append(custom);
    }
    return mCustomEmojies;
}

void RocketChatAccount::loadEmoji(const QJsonObject &obj)
{
    mEmojiManager->loadCustomEmoji(obj);
    updateCustomEmojiList(false);
}

void RocketChatAccount::updateCustomEmojiList(bool fetchListCustom)
{
    if (fetchListCustom) {
        listEmojiCustom();
    } else {
        const auto customEmojiList = mEmojiManager->customEmojiList();
        mEmoticonModel->setCustomEmojiList(customEmojiList);
        if (Ruqola::self()->customEmojiIconManager()->currentRocketChatAccount() == this) {
            TextEmoticonsCore::EmojiModelManager::self()->emojiModel()->setCustomEmojiList(customEmojies());
        }
    }
}

OtrManager *RocketChatAccount::otrManager() const
{
    return mOtrManager;
}

void RocketChatAccount::deleteMessage(const QString &messageId, const QString &roomId)
{
    restApi()->deleteMessage(roomId, messageId);
}

void RocketChatAccount::insertCompleterUsers()
{
    userCompleterModel()->insertUsers(rocketChatBackend()->users());
}

void RocketChatAccount::userAutocomplete(const QString &searchText, const QString &exception)
{
    userCompleterModel()->clear();
    if (!searchText.isEmpty()) {
        RocketChatRestApi::UsersAutocompleteJob::UsersAutocompleterInfo info;
        info.pattern = searchText;
        info.exception = exception;
        restApi()->usersAutocomplete(info);
    }
}

void RocketChatAccount::membersInRoom(const QString &roomId, Room::RoomType channelType)
{
    // We call it first for initialize all member in rooms.
    // We need to clear it. It can be initialize by "/rooms-changed" signal
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    if (usersModelForRoom) {
        if (usersModelForRoom->total() != 0) {
            usersModelForRoom->clear();
        }
        usersModelForRoom->setLoadMoreUsersInProgress(true);
    }
    restApi()->membersInRoom(roomId, Room::roomFromRoomType(channelType));
}

void RocketChatAccount::updateUserInRoom(const QJsonObject &roomData)
{
    const QString roomId = roomData.value(QStringLiteral("_id")).toString();
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    if (usersModelForRoom) {
        const int numberOfUsers = roomData.value(QStringLiteral("usersCount")).toInt();
        if (usersModelForRoom->total() != numberOfUsers) {
            if (!usersModelForRoom->loadMoreUsersInProgress()) {
                usersModelForRoom->clear();
                usersModelForRoom->setLoadMoreUsersInProgress(true);
                const QString channelType = roomData.value(QStringLiteral("t")).toString();
                restApi()->membersInRoom(roomId, channelType, 0, qMin(50, usersModelForRoom->offset()));
            }
        }
    }
}

void RocketChatAccount::parseUsersForRooms(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    // FIXME channelInfo
    const QString channelInfoIdentifier = channelInfo.identifier;
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(channelInfoIdentifier);
    if (usersModelForRoom) {
        usersModelForRoom->parseUsersForRooms(obj, mUserModel, true);
        usersModelForRoom->setLoadMoreUsersInProgress(false);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << channelInfoIdentifier;
    }
}

void RocketChatAccount::roomFiles(const QString &roomId, Room::RoomType channelType)
{
    mFilesModelForRoom->initialize();
    restApi()->filesInRoom(roomId, Room::roomFromRoomType(channelType));
}

MessagesModel *RocketChatAccount::threadMessageModel() const
{
    return mThreadMessageModel;
}

DiscussionsModel *RocketChatAccount::discussionsModel() const
{
    return mDiscussionsModel;
}

FilesForRoomModel *RocketChatAccount::filesModelForRoom() const
{
    return mFilesModelForRoom;
}

EmoticonModel *RocketChatAccount::emoticonModel() const
{
    return mEmoticonModel;
}

CommandsModel *RocketChatAccount::commandsModel() const
{
    return mCommandsModel;
}

ReceiveTypingNotificationManager *RocketChatAccount::receiveTypingNotificationManager() const
{
    return mReceiveTypingNotificationManager;
}

void RocketChatAccount::slotChannelGroupRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    Room *room = mRoomModel->findRoom(channelInfo.identifier);
    if (room) {
        Roles r;
        r.parseRole(obj);
        room->setRolesForRooms(r);
        // qDebug() << " r " << r << " room " << room->name() << " obj" << obj;
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << channelInfo.identifier;
    }
}

void RocketChatAccount::slotGetThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId)
{
    if (mThreadMessageModel->threadMessageId() != threadMessageId) {
        mThreadMessageModel->setThreadMessageId(threadMessageId);
        mThreadMessageModel->parseThreadMessages(obj);
    } else {
        mThreadMessageModel->loadMoreThreadMessages(obj);
    }
}

void RocketChatAccount::slotGetDiscussionsListDone(const QJsonObject &obj, const QString &roomId)
{
    if (mDiscussionsModel->roomId() != roomId) {
        mDiscussionsModel->parseDiscussions(obj, roomId);
    } else {
        mDiscussionsModel->addMoreDiscussions(obj);
    }
    mDiscussionsModel->setLoadMoreDiscussionsInProgress(false);
}

void RocketChatAccount::slotGetListMessagesDone(const QJsonObject &obj, const QString &roomId, ListMessagesModel::ListMessageType type)
{
    if (mMarkUnreadThreadsAsReadOnNextReply && type == ListMessagesModel::UnreadThreadsMessages) {
        qCDebug(RUQOLA_THREAD_MESSAGE_LOG) << "Obj" << obj << "roomId:" << roomId;
        mMarkUnreadThreadsAsReadOnNextReply = false;

        ListMessages messages;
        messages.parseMessages(obj, QStringLiteral("threads"));
        const auto listMessages = messages.listMessages();
        for (const auto &msg : listMessages) {
            QJsonObject params;
            params.insert(QStringLiteral("tmid"), msg.messageId());
            mDdp->method(QStringLiteral("getThreadMessages"), QJsonDocument(params), [](const QJsonObject &reply, RocketChatAccount *account) {
                // don't trigger warning about unhandled replies
                Q_UNUSED(reply)
                Q_UNUSED(account)
            });
        }
        return;
    }

    if (mListMessageModel->roomId() != roomId || mListMessageModel->listMessageType() != type) {
        mListMessageModel->setRoomId(roomId);
        mListMessageModel->setListMessageType(type);
        mListMessageModel->parseListMessages(obj);
    } else {
        mListMessageModel->loadMoreListMessages(obj);
    }
    mListMessageModel->setLoadMoreListMessagesInProgress(false);
}

void RocketChatAccount::slotUserAutoCompleterDone(const QJsonObject &obj)
{
    const QVector<User> users = User::parseUsersList(obj, roleInfo());
    mUserCompleterModel->insertUsers(users);
}

User::PresenceStatus RocketChatAccount::presenceStatus() const
{
    return mPresenceStatus;
}

AccountRoomSettings *RocketChatAccount::accountRoomSettings() const
{
    return mAccountRoomSettings;
}

ListMessagesFilterProxyModel *RocketChatAccount::listMessagesFilterProxyModel() const
{
    return mListMessagesFilterProxyModel;
}

ListMessagesModel *RocketChatAccount::listMessageModel() const
{
    return mListMessageModel;
}

void RocketChatAccount::slotDirectoryDone(const QJsonObject &obj)
{
    qDebug() << "void RocketChatAccount::slotDirectoryDone(const QJsonObject &obj)" << obj;
    mSearchChannelModel->parseChannels(obj);
}

void RocketChatAccount::slotSplotLightDone(const QJsonObject &obj)
{
    // qDebug() << " void RocketChatAccount::slotSplotLightDone(const QJsonObject &obj)"<<obj;
    // If empty ! show empty list
    mSearchChannelModel->parseChannels(obj);
}

void RocketChatAccount::slotChannelListDone(const QJsonObject &obj)
{
    // qDebug() << " void RocketChatAccount::slotChannelListDone(const QJsonObject &obj)" << obj;
    mSearchChannelModel->parseAllChannels(obj);
}

void RocketChatAccount::slotChannelFilesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    // TODO fixme channelinfo
    // qDebug() << " slotChannelFilesDone(const QJsonObject &obj, const QString &roomId)" << roomId << " obj " << obj;
    if (mFilesModelForRoom->roomId() != channelInfo.identifier) {
        mFilesModelForRoom->parseFileAttachments(obj, channelInfo.identifier);
    } else {
        mFilesModelForRoom->addMoreFileAttachments(obj);
    }
    mFilesModelForRoom->setLoadMoreFilesInProgress(false);
}

void RocketChatAccount::loadMoreUsersInRoom(const QString &roomId, Room::RoomType channelType)
{
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    const int offset = usersModelForRoom->usersCount();
    if (offset < usersModelForRoom->total()) {
        usersModelForRoom->setLoadMoreUsersInProgress(true);
        restApi()->membersInRoom(roomId, Room::roomFromRoomType(channelType), offset, qMin(50, usersModelForRoom->total() - offset));
    }
}

void RocketChatAccount::getMentionsMessages(const QString &roomId)
{
    mListMessageModel->clear();
    mListMessageModel->setRoomId(roomId);
    mListMessageModel->setLoadMoreListMessagesInProgress(true);
    restApi()->getMentionedMessages(roomId);
}

void RocketChatAccount::getPinnedMessages(const QString &roomId)
{
    mListMessageModel->clear();
    mListMessageModel->setLoadMoreListMessagesInProgress(true);
    mListMessageModel->setRoomId(roomId);
    restApi()->getPinnedMessages(roomId);
}

void RocketChatAccount::getStarredMessages(const QString &roomId)
{
    mListMessageModel->clear();
    mListMessageModel->setRoomId(roomId);
    mListMessageModel->setLoadMoreListMessagesInProgress(true);
    restApi()->getStarredMessages(roomId);
}

void RocketChatAccount::loadMoreFileAttachments(const QString &roomId, Room::RoomType channelType)
{
    if (!mFilesModelForRoom->loadMoreFilesInProgress()) {
        const int offset = mFilesModelForRoom->fileAttachments()->filesCount();
        if (offset < mFilesModelForRoom->fileAttachments()->total()) {
            mFilesModelForRoom->setLoadMoreFilesInProgress(true);
            restApi()->filesInRoom(roomId, Room::roomFromRoomType(channelType), offset, qMin(50, mFilesModelForRoom->fileAttachments()->total() - offset));
        }
    }
}

void RocketChatAccount::loadMoreDiscussions(const QString &roomId)
{
    if (!mDiscussionsModel->loadMoreDiscussionsInProgress()) {
        const int offset = mDiscussionsModel->discussions()->discussionsCount();
        if (offset < mDiscussionsModel->discussions()->total()) {
            mDiscussionsModel->setLoadMoreDiscussionsInProgress(true);
            restApi()->getDiscussions(roomId, offset, qMin(50, mDiscussionsModel->discussions()->total() - offset));
        }
    }
}

void RocketChatAccount::updateThreadMessageList(const Message &m)
{
    if (mThreadMessageModel->threadMessageId() == m.threadMessageId()) {
        mThreadMessageModel->addMessages({m});
    }
}

void RocketChatAccount::getListMessages(const QString &roomId, ListMessagesModel::ListMessageType type)
{
    mListMessageModel->setListMessageType(type);
    mListMessageModel->setLoadMoreListMessagesInProgress(true);
    switch (type) {
    case ListMessagesModel::Unknown:
        qCWarning(RUQOLA_LOG) << " Error when using getListMessages";
        break;
    case ListMessagesModel::StarredMessages:
        getStarredMessages(roomId);
        break;
    case ListMessagesModel::PinnedMessages:
        getPinnedMessages(roomId);
        break;
    case ListMessagesModel::MentionsMessages:
        getMentionsMessages(roomId);
        break;
    case ListMessagesModel::ThreadsMessages:
        threadsInRoom(roomId, false);
        break;
    case ListMessagesModel::UnreadThreadsMessages:
        threadsInRoom(roomId, true);
        break;
    }
}

void RocketChatAccount::setNameChanged(const QJsonArray &array)
{
    qCWarning(RUQOLA_LOG) << "Need to implement: Users:NameChanged :" << array << " account name " << accountName();
    // QJsonArray([{"_id":"Z5TPBsWrmjAWCKGBC","name":"LifeLine","username":"LifeLine-GM"}])
    for (int i = 0; i < array.count(); ++i) {
        const QJsonObject obj = array.at(i).toObject();
        const QString id = obj[QLatin1String("_id")].toString();
        const QString name = obj[QLatin1String("name")].toString();
        const QString username = obj[QLatin1String("username")].toString();
        // TODO
    }
}

void RocketChatAccount::setOwnStatus(const User &user)
{
    userStatusChanged(user);
}

void RocketChatAccount::setUserStatusChanged(const QJsonArray &array)
{
    // qDebug() << "Account Name : " << accountName() << " status changed: " << array << " array " << array.count() << " array" << array.toVariantList();
    const auto list = array.toVariantList();
    for (const auto &var : list) {
        const auto userListArguments = var.toJsonArray().toVariantList();
        User user;
        user.parseUser(userListArguments);
        if (user.isValid()) {
            userStatusChanged(user);
            // qDebug() << " user status changed " << user;
        }
    }
}

void RocketChatAccount::setShowRoomAvatar(bool checked)
{
    RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    info.userId = userId();
    info.sidebarDisplayAvatar = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(checked);
    setUserPreferences(std::move(info));
}

void RocketChatAccount::setShowFavoriteRoom(bool checked)
{
    RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    info.userId = userId();
    info.sidebarShowFavorites = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(checked);
    setUserPreferences(std::move(info));
}

void RocketChatAccount::loadMoreListMessages(const QString &roomId)
{
    if (!mListMessageModel->loadMoreListMessagesInProgress()) {
        const int offset = mListMessageModel->rowCount();
        if (offset < mListMessageModel->total()) {
            mListMessageModel->setLoadMoreListMessagesInProgress(true);
            switch (mListMessageModel->listMessageType()) {
            case ListMessagesModel::Unknown:
                qCWarning(RUQOLA_LOG) << " Error when using loadMoreListMessages";
                break;
            case ListMessagesModel::StarredMessages:
                restApi()->getStarredMessages(roomId, offset, qMin(50, mListMessageModel->total() - offset));
                break;
            case ListMessagesModel::PinnedMessages:
                restApi()->getPinnedMessages(roomId, offset, qMin(50, mListMessageModel->total() - offset));
                break;
            case ListMessagesModel::MentionsMessages:
                restApi()->getMentionedMessages(roomId, offset, qMin(50, mListMessageModel->total() - offset));
                break;
            case ListMessagesModel::ThreadsMessages:
                restApi()->getThreadsList(roomId, false, offset, qMin(50, mListMessageModel->total() - offset));
                break;
            case ListMessagesModel::UnreadThreadsMessages:
                restApi()->getThreadsList(roomId, true, offset, qMin(50, mListMessageModel->total() - offset));
                break;
            }
        }
    }
}

void RocketChatAccount::loadThreadMessagesHistory(const QString &threadMessageId)
{
    restApi()->getThreadMessages(threadMessageId);
}

void RocketChatAccount::createJitsiConfCall(const QString &roomId)
{
    // TODO use restapi
    ddp()->createJitsiConfCall(roomId);
    joinJitsiConfCall(roomId);
}

void RocketChatAccount::addUserToRoom(const QString &userId, const QString &roomId, Room::RoomType channelType)
{
    switch (channelType) {
    case Room::RoomType::Private:
        restApi()->addUserInGroup(roomId, userId);
        break;
    case Room::RoomType::Channel:
        restApi()->addUserInChannel(roomId, userId);
        break;
    case Room::RoomType::Direct:
    case Room::RoomType::Unknown:
        break;
    }
}

void RocketChatAccount::clearSearchModel()
{
    mSearchMessageModel->clearModel();
}

void RocketChatAccount::messageSearch(const QString &pattern, const QString &rid, bool userRegularExpression)
{
    if (pattern.isEmpty()) {
        clearSearchModel();
    } else {
        mSearchMessageModel->setLoadCommonMessagesInProgress(true);
        restApi()->searchMessages(rid, pattern, userRegularExpression);
    }
}

InputTextManager *RocketChatAccount::inputTextManager() const
{
    return mInputTextManager;
}

InputTextManager *RocketChatAccount::inputThreadMessageTextManager() const
{
    return mInputThreadMessageTextManager;
}

void RocketChatAccount::slotSearchMessages(const QJsonObject &obj)
{
    mSearchMessageModel->setLoadCommonMessagesInProgress(false);
    mSearchMessageModel->parse(obj);
}

void RocketChatAccount::starMessage(const QString &messageId, bool starred)
{
    restApi()->starMessage(messageId, starred);
}

void RocketChatAccount::pinMessage(const QString &messageId, bool pinned)
{
    restApi()->pinMessage(messageId, pinned);
}

void RocketChatAccount::reportMessage(const QString &messageId, const QString &message)
{
    restApi()->reportMessage(messageId, message);
}

void RocketChatAccount::getThreadMessages(const QString &threadMessageId, const Message &message)
{
    // mListMessageModel->clear();
    mThreadMessageModel->setPreviewMessage(message);
    restApi()->getThreadMessages(threadMessageId);
}

void RocketChatAccount::changeNotificationsSettings(const QString &roomId,
                                                    RocketChatAccount::NotificationOptionsType notificationsType,
                                                    const QVariant &newValue)
{
    switch (notificationsType) {
    case DisableNotifications:
        restApi()->disableNotifications(roomId, newValue.toBool());
        break;
    case HideUnreadStatus:
        restApi()->hideUnreadStatus(roomId, newValue.toBool());
        break;
    case DesktopNotifications:
        restApi()->desktopNotifications(roomId, newValue.toString());
        break;
    case EmailNotifications:
        restApi()->emailNotifications(roomId, newValue.toString());
        break;
    case MobilePushNotifications:
        restApi()->mobilePushNotifications(roomId, newValue.toString());
        break;
    case UnreadAlert:
        restApi()->unreadAlert(roomId, newValue.toString());
        break;
    case MuteGroupMentions:
        restApi()->muteGroupMentions(roomId, newValue.toBool());
        break;
    case DesktopDurationNotifications:
        restApi()->desktopDurationNotifications(roomId, newValue.toInt());
        break;
    case DesktopSoundNotifications:
        restApi()->desktopSoundNotifications(roomId, newValue.toString());
        break;
    case HideMentionStatus:
        restApi()->hideMentionStatus(roomId, newValue.toBool());
        break;
    }
}

void RocketChatAccount::parsePublicSettings(const QJsonObject &obj, bool update)
{
    mRuqolaServerConfig->parsePublicSettings(obj, update);
    parsePublicSettings();
}

void RocketChatAccount::parsePublicSettings()
{
    localDatabaseManager()->updateAccount(accountName(), mRuqolaServerConfig->serialize(false), LocalDatabaseUtils::currentTimeStamp());

    fillOauthModel();
    // Download logo/favicon if possible
    (void)faviconLogoUrlFromLocalCache(mRuqolaServerConfig->logoUrl().url);
    (void)faviconLogoUrlFromLocalCache(mRuqolaServerConfig->faviconUrl().url);

    Q_EMIT publicSettingChanged();
}

void RocketChatAccount::fillOauthModel()
{
    QVector<AuthenticationInfo> fillModel;
    for (int i = 0, total = mLstInfos.count(); i < total; ++i) {
        if (mRuqolaServerConfig->canShowOauthService(mLstInfos.at(i).oauthType())) {
            fillModel.append(mLstInfos.at(i));
        }
    }
    mLoginMethodModel->setAuthenticationInfos(fillModel);
}

void RocketChatAccount::changeDefaultAuthentication(int index)
{
    setDefaultAuthentication(mLoginMethodModel->loginType(index));
}

void RocketChatAccount::setDefaultAuthentication(AuthenticationManager::OauthType type)
{
    PluginAuthenticationInterface *interface = mLstPluginAuthenticationInterface.value(type);
    if (interface) {
        mDefaultAuthenticationInterface = interface;
    } else {
        qCWarning(RUQOLA_LOG) << "No interface defined for  " << type;
    }
}

CommonMessageFilterProxyModel *RocketChatAccount::searchMessageFilterProxyModel() const
{
    return mSearchMessageFilterProxyModel;
}

CommonMessagesModel *RocketChatAccount::searchMessageModel() const
{
    return mSearchMessageModel;
}

void RocketChatAccount::initializeAuthenticationPlugins()
{
    // TODO change it when we change server
    // Clean up at the end.
    const QVector<PluginAuthentication *> lstPlugins = AuthenticationManager::self()->pluginsList();
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::initializeAuthenticationPlugins()" << lstPlugins.count();
    if (lstPlugins.isEmpty()) {
        qCWarning(RUQOLA_LOG) << " No plugins loaded. Please verify your installation.";
        ddp()->authenticationManager()->setLoginStatus(DDPAuthenticationManager::FailedToLoginPluginProblem);
        return;
    }
    mLstPluginAuthenticationInterface.clear();

    mLstInfos.clear();
    for (PluginAuthentication *abstractPlugin : lstPlugins) {
        AuthenticationInfo info;
        info.setIconName(abstractPlugin->iconName());
        info.setName(abstractPlugin->name());
        info.setOauthType(abstractPlugin->type());
        if (info.isValid()) {
            mLstInfos.append(std::move(info));
        }

        PluginAuthenticationInterface *interface = abstractPlugin->createInterface(this);
        interface->setAccount(this);
        mRuqolaServerConfig->addRuqolaAuthenticationSupport(abstractPlugin->type());
        mLstPluginAuthenticationInterface.insert(abstractPlugin->type(), interface);
        // For the moment initialize default interface
        if (abstractPlugin->type() == AuthenticationManager::OauthType::Password) {
            mDefaultAuthenticationInterface = interface;
        }
        qCDebug(RUQOLA_LOG) << " plugin type " << abstractPlugin->type();
    }
    // TODO fill ??? or store QVector<AuthenticationInfo>
}

PluginAuthenticationInterface *RocketChatAccount::defaultAuthenticationInterface() const
{
    return mDefaultAuthenticationInterface;
}

LoginMethodModel *RocketChatAccount::loginMethodModel() const
{
    return mLoginMethodModel;
}

QString RocketChatAccount::authToken() const
{
    return settings()->authToken();
}

QString RocketChatAccount::userName() const
{
    return settings()->userName();
}

void RocketChatAccount::setAccountName(const QString &accountname)
{
    // Initialize new account room
    // qDebug() << "void RocketChatAccount::setAccountName(const QString &accountname)"<<accountname;
    loadSettings(ManagerDataPaths::self()->accountConfigFileName(accountname));
    settings()->setAccountName(accountname);
}

QString RocketChatAccount::accountName() const
{
    return settings()->accountName();
}

QString RocketChatAccount::displayName() const
{
    return settings()->displayName();
}

void RocketChatAccount::addUpdateEmojiCustomList(const QJsonArray &replyArray)
{
    mEmojiManager->addUpdateEmojiCustomList(replyArray);
}

void RocketChatAccount::deleteEmojiCustom(const QJsonArray &replyArray)
{
    mEmojiManager->deleteEmojiCustom(replyArray);
}

void RocketChatAccount::privateSettingsUpdated(const QJsonArray &replyArray)
{
    mRuqolaServerConfig->privateSettingsUpdated(replyArray);
    localDatabaseManager()->updateAccount(accountName(), mRuqolaServerConfig->serialize(false), LocalDatabaseUtils::currentTimeStamp());
    Q_EMIT privateSettingsChanged();
}

void RocketChatAccount::permissionUpdated(const QJsonArray &replyArray)
{
    if (mPermissionManager.updatePermission(replyArray)) {
        Q_EMIT permissionChanged();
    }
    // TODO stockage roles! Load it before permission!
    // QJsonObject({"args":["updated",{"_id":"access-mailer","_updatedAt":{"$date":1634569746270},"roles":["admin","vFXCWG9trXLti6xQm"]}],"eventName":"permissions-changed"})
}

const QVector<RoleInfo> &RocketChatAccount::roleInfo() const
{
    return mRolesManager.roleInfo();
}

void RocketChatAccount::deleteCustomSound(const QJsonArray &replyArray)
{
    mCustomSoundManager->deleteCustomSounds(replyArray);
}

void RocketChatAccount::updateRoles(const QJsonArray &contents)
{
    mRolesManager.updateRoles(contents);
}

void RocketChatAccount::addStdoutInfo(const QJsonArray &contents)
{
    const auto count{contents.count()};
    for (auto i = 0; i < count; ++i) {
        const QJsonObject obj = contents.at(i).toObject();
        const QString infoStr = obj[QLatin1String("string")].toString();
        // qDebug() << " infoStr " << infoStr;
        Q_EMIT insertStdOutInfo(infoStr);
    }
}

void RocketChatAccount::updateCustomSound(const QJsonArray &replyArray)
{
    mCustomSoundManager->updateCustomSounds(replyArray);
}

void RocketChatAccount::deleteUser(const QJsonArray &replyArray)
{
    qDebug() << " void RocketChatAccount::deleteUser(const QJsonObject &replyObject)" << replyArray;
}

void RocketChatAccount::deleteCustomUserStatus(const QJsonArray &replyArray)
{
    qDebug() << " void RocketChatAccount::deleteCustomUserStatus(const QJsonObject &replyObject)" << replyArray;
    mCustomUserStatuses.deleteCustomUserStatuses(replyArray);
    Q_EMIT customUserStatusChanged();
}

void RocketChatAccount::updateCustomUserStatus(const QJsonArray &replyArray)
{
    mCustomUserStatuses.updateCustomUserStatues(replyArray);
    Q_EMIT customUserStatusChanged();
    qDebug() << " void RocketChatAccount::updateCustomUserStatus(const QJsonObject &replyObject)" << replyArray;
}

void RocketChatAccount::setDisplayName(const QString &displayName)
{
    settings()->setDisplayName(displayName);
}

QString RocketChatAccount::userId() const
{
    return settings()->userId();
}

QString RocketChatAccount::password() const
{
    return settings()->password();
}

QString RocketChatAccount::twoFactorAuthenticationCode() const
{
    return settings()->twoFactorAuthenticationCode();
}

void RocketChatAccount::setAuthToken(const QString &token)
{
    settings()->setAuthToken(token);
}

void RocketChatAccount::setPassword(const QString &password)
{
    settings()->setPassword(password);
}

void RocketChatAccount::setTwoFactorAuthenticationCode(const QString &twoFactorAuthenticationCode)
{
    settings()->setTwoFactorAuthenticationCode(twoFactorAuthenticationCode);
}

void RocketChatAccount::setAccountEnabled(bool enabled)
{
    settings()->setAccountEnabled(enabled);
}

void RocketChatAccount::setUserName(const QString &username)
{
    settings()->setUserName(username);
}

bool RocketChatAccount::accountEnabled() const
{
    return settings()->accountEnabled();
}

void RocketChatAccount::setUserId(const QString &userID)
{
    settings()->setUserId(userID);
}

QString RocketChatAccount::serverUrl() const
{
    return settings()->serverUrl();
}

void RocketChatAccount::setServerUrl(const QString &serverURL)
{
    settings()->setServerUrl(serverURL);
    restApi()->setServerUrl(serverURL);
    mEmojiManager->setServerUrl(serverURL);
}

QUrl RocketChatAccount::urlForLink(const QString &link) const
{
    if (link.startsWith(QLatin1String("https:")) || link.startsWith(QLatin1String("http:"))) {
        return QUrl(link);
    }
    QString tmpUrl = settings()->serverUrl();
    if (!tmpUrl.startsWith(QLatin1String("https://"))) {
        tmpUrl = QLatin1String("https://") + tmpUrl;
    }
    if (!link.startsWith(QLatin1Char('/'))) {
        tmpUrl += QLatin1Char('/');
    }
    const QUrl downloadFileUrl = QUrl::fromUserInput(tmpUrl + link);
    return downloadFileUrl;
}

QString RocketChatAccount::recordingVideoPath() const
{
    return mCache->recordingVideoPath(accountName());
}

QString RocketChatAccount::recordingImagePath() const
{
    return mCache->recordingImagePath(accountName());
}

void RocketChatAccount::downloadFile(const QString &downloadFileUrl, const QUrl &localFile)
{
    mCache->downloadFile(downloadFileUrl, localFile);
}

QUrl RocketChatAccount::faviconLogoUrlFromLocalCache(const QString &url)
{
    return mCache->faviconLogoUrlFromLocalCache(url);
}

QUrl RocketChatAccount::attachmentUrlFromLocalCache(const QString &url)
{
    return mCache->attachmentUrlFromLocalCache(url);
}

bool RocketChatAccount::attachmentIsInLocalCache(const QString &url)
{
    return mCache->attachmentIsInLocalCache(url);
}

void RocketChatAccount::loadHistory(const QString &roomID, bool initial, qint64 timeStamp)
{
    MessagesModel *roomModel = messageModelForRoom(roomID);
    if (roomModel) {
        Room *room = mRoomModel->findRoom(roomID);
        // qDebug() << " room->numberMessages() " << room->numberMessages() << " roomModel->rowCount() " << roomModel->rowCount();
        if (!initial && (room->numberMessages() == roomModel->rowCount())) {
            return;
        }
        ManageLocalDatabase::ManageLoadHistoryInfo info;
        info.roomModel = roomModel;
        info.roomId = roomID;
        info.initial = initial;
        info.timeStamp = timeStamp;
        info.roomName = room->displayFName();
        info.lastSeenAt = room->lastSeenAt();
        mManageLoadHistory->loadMessagesHistory(info);
    } else {
        qCWarning(RUQOLA_LOG) << "Room is not found " << roomID;
    }
}

void RocketChatAccount::loadAccountSettings()
{
    mManageLoadHistory->loadAccountSettings();
}

void RocketChatAccount::setServerVersion(const QString &version)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::setServerVersion(const QString &version)" << version;
    mRuqolaServerConfig->setServerVersion(version);
    Q_EMIT serverVersionChanged();
}

QString RocketChatAccount::serverVersion() const
{
    return mRuqolaServerConfig->serverVersion();
}

bool RocketChatAccount::needAdaptNewSubscriptionRC60() const
{
    return mRuqolaServerConfig->needAdaptNewSubscriptionRC60();
}

bool RocketChatAccount::otrEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::OtrEnabled;
}

bool RocketChatAccount::allowProfileChange() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowUserProfileChange;
}

void RocketChatAccount::enable2FaEmailJob(bool enable)
{
    restApi()->enable2FaEmailJob(enable);
}

bool RocketChatAccount::allowMessagePinningEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowMessagePinning;
}

bool RocketChatAccount::allowMessageStarringEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowMessageStarring;
}

bool RocketChatAccount::allowMessageDeletingEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowMessageDeleting;
}

bool RocketChatAccount::threadsEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::ThreadsEnabled;
}

bool RocketChatAccount::autoTranslateEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AutoTranslateEnabled;
}

bool RocketChatAccount::encryptionEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::EncryptionEnabled;
}

bool RocketChatAccount::twoFactorAuthenticationEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::TwoFactorAuthenticationEnabled;
}

bool RocketChatAccount::twoFactorAuthenticationByEmailEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::TwoFactorAuthenticationByEmailEnabled;
}

bool RocketChatAccount::twoFactorAuthenticationEnforcePasswordFallback() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::TwoFactorAuthenticationEnforcePasswordFallback;
}

bool RocketChatAccount::twoFactorAuthenticationByTOTPEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::TwoFactorAuthenticationByTOTPEnabled;
}

bool RocketChatAccount::broadCastEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::BroadCastEnabled;
}

bool RocketChatAccount::registrationFromEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::RegistrationFromEnabled;
}

bool RocketChatAccount::allowDeleteOwnAccount() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowDeleteOwnAccount;
}

bool RocketChatAccount::discussionEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::DiscussionEnabled;
}

bool RocketChatAccount::allowAvatarChanged() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowUserAvatarChange;
}

bool RocketChatAccount::audioRecorderEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AudioRecorderEnabled;
}

bool RocketChatAccount::videoRecorderEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::VideoRecorderEnabled;
}

bool RocketChatAccount::teamEnabled() const
{
    return hasPermission(QStringLiteral("create-team"));
}

bool RocketChatAccount::ldapEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::LdapEnabled;
}

ServerConfigInfo *RocketChatAccount::serverConfigInfo() const
{
    return mServerConfigInfo;
}

bool RocketChatAccount::jitsiEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::JitsiEnabled;
}

void RocketChatAccount::groupInfo(const QString &roomId)
{
    restApi()->groupInfo(roomId);
}

void RocketChatAccount::switchEditingMode(bool b)
{
    if (mEditingMode != b) {
        mEditingMode = b;
        Q_EMIT editingModeChanged();
    }
}

void RocketChatAccount::setSortUnreadOnTop(bool checked)
{
    RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    info.userId = userId();
    info.sidebarShowUnread = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(checked);
    setUserPreferences(std::move(info));
}

bool RocketChatAccount::sortUnreadOnTop() const
{
    return ownUser().ownUserPreferences().showUnread();
}

bool RocketChatAccount::sortFavoriteChannels() const
{
    return ownUser().ownUserPreferences().showFavorite();
}

void RocketChatAccount::setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder roomListSortOrder)
{
    RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    info.userId = userId();
    switch (roomListSortOrder) {
    case OwnUserPreferences::RoomListSortOrder::ByLastMessage:
        info.sidebarSortby = QStringLiteral("activity");
        break;
    case OwnUserPreferences::RoomListSortOrder::Alphabetically:
        info.sidebarSortby = QStringLiteral("alphabetical");
        break;
    case OwnUserPreferences::RoomListSortOrder::Unknown:
        qCWarning(RUQOLA_LOG) << " OwnUserPreferences::RoomListSortOrder::Unknown is a bug";
        return;
    }
    setUserPreferences(std::move(info));
}

OwnUserPreferences::RoomListSortOrder RocketChatAccount::roomListSortOrder() const
{
    return ownUser().ownUserPreferences().roomListSortOrder();
}

void RocketChatAccount::kickUser(const QString &roomId, const QString &userId, Room::RoomType channelType)
{
    switch (channelType) {
    case Room::RoomType::Private:
        restApi()->groupKick(roomId, userId);
        break;
    case Room::RoomType::Channel:
        restApi()->channelKick(roomId, userId);
        break;
    case Room::RoomType::Direct:
        break;
    case Room::RoomType::Unknown:
        qCWarning(RUQOLA_LOG) << " unsupported kickUser room for type " << channelType;
        break;
    }
}

void RocketChatAccount::rolesInRoom(const QString &roomId, Room::RoomType channelType)
{
    switch (channelType) {
    case Room::RoomType::Private:
        restApi()->getGroupRoles(roomId);
        break;
    case Room::RoomType::Channel:
        restApi()->getChannelRoles(roomId);
        break;
    case Room::RoomType::Direct:
        break;
    case Room::RoomType::Unknown:
        qCWarning(RUQOLA_LOG) << " unsupported get roles room for type " << channelType;
        break;
    }
}

void RocketChatAccount::switchingToRoom(const QString &roomID)
{
    clearTypingNotification();
    checkInitializedRoom(roomID);
}

void RocketChatAccount::changeRoles(const QString &roomId, const QString &userId, Room::RoomType channelType, RocketChatAccount::RoleType roleType)
{
    switch (channelType) {
    case Room::RoomType::Private:
        switch (roleType) {
        case RocketChatAccount::AddOwner:
            restApi()->groupAddOwner(roomId, userId);
            break;
        case RocketChatAccount::AddLeader:
            restApi()->groupAddLeader(roomId, userId);
            break;
        case RocketChatAccount::AddModerator:
            restApi()->groupAddModerator(roomId, userId);
            break;
        case RocketChatAccount::RemoveOwner:
            restApi()->groupRemoveOwner(roomId, userId);
            break;
        case RocketChatAccount::RemoveLeader:
            restApi()->groupRemoveLeader(roomId, userId);
            break;
        case RocketChatAccount::RemoveModerator:
            restApi()->groupRemoveModerator(roomId, userId);
            break;
        }

        break;
    case Room::RoomType::Channel:
        switch (roleType) {
        case RocketChatAccount::AddOwner:
            restApi()->channelAddOwner(roomId, userId);
            break;
        case RocketChatAccount::AddLeader:
            restApi()->channelAddLeader(roomId, userId);
            break;
        case RocketChatAccount::AddModerator:
            restApi()->channelAddModerator(roomId, userId);
            break;
        case RocketChatAccount::RemoveOwner:
            restApi()->channelRemoveOwner(roomId, userId);
            break;
        case RocketChatAccount::RemoveLeader:
            restApi()->channelRemoveLeader(roomId, userId);
            break;
        case RocketChatAccount::RemoveModerator:
            restApi()->channelRemoveModerator(roomId, userId);
            break;
        }

        break;
    case Room::RoomType::Direct:
    case Room::RoomType::Unknown:
        qCWarning(RUQOLA_LOG) << " unsupported changeRoles room for type " << channelType;
        break;
    }
}

void RocketChatAccount::channelInfo(const QString &roomId)
{
    restApi()->channelInfo(roomId);
}

bool RocketChatAccount::allowEditingMessages() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowEditingMessage;
}

bool RocketChatAccount::uploadFileEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::UploadFileEnabled;
}

bool RocketChatAccount::isMessageEditable(const Message &message) const
{
    if (!allowEditingMessages()) {
        return false;
    }
    if (hasPermission(QStringLiteral("edit-message"))) {
        return true;
    }
    if (message.userId() != userId()) {
        return false;
    }
    if (ruqolaServerConfig()->blockEditingMessageInMinutes() == 0) {
        return true;
    }
    constexpr int minute = 60 * 1000;
    return (message.timeStamp() + ruqolaServerConfig()->blockEditingMessageInMinutes() * minute) > QDateTime::currentMSecsSinceEpoch();
}

bool RocketChatAccount::isMessageDeletable(const Message &message) const
{
    if (!allowMessageDeletingEnabled()) {
        return false;
    }
    if (hasPermission(QStringLiteral("force-delete-message"))) {
        return true;
    }
    if (hasPermission(QStringLiteral("delete-message"))) {
        return true;
    }
    if (message.userId() != userId()) {
        return false;
    }
    if (ruqolaServerConfig()->blockDeletingMessageInMinutes() == 0) { // TODO verify it
        return true;
    }
    constexpr int minutes = 60 * 1000;
    return (message.timeStamp() + ruqolaServerConfig()->blockDeletingMessageInMinutes() * minutes) > QDateTime::currentMSecsSinceEpoch();
}

void RocketChatAccount::parseVideoConference(const QJsonArray &contents)
{
    qDebug() << " RocketChatAccount::parseVideoConference(const QJsonArray &contents) " << contents;
    mVideoConferenceManager->parseVideoConference(contents);
}

void RocketChatAccount::parseOtr(const QJsonArray &contents)
{
    qDebug() << " void RocketChatAccount::parseOtr(const QJsonArray &contents)" << contents << " account name" << accountName();
    mOtrManager->parseOtr(contents);
}

void RocketChatAccount::sendNotification(const QJsonArray &contents)
{
    // Conference call
    // NOTIFICATION:  QJsonObject({"collection":"stream-notify-user","fields":
    // {"args":[{"payload":{"_id":"vD3CXNB2oK5uB8nDT","message":{"msg":"","t":"videoconf"},
    // "rid":"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K","sender":{"_id":"YbwG4T2uB3wZSZSKB",
    // "name":"laurent","username":"laurent-montel"},"type":"d"},"text":"","title":"laurent"}],
    // "eventName":"xkNpoB3T98EEPCj2K/notification"},"id":"id","msg":"changed"})

    NotificationInfo info;
    info.setAccountName(accountName());
    info.setDateTime(QDateTime::currentDateTime().toString());
    info.parseNotification(contents);
    if (!info.isValid()) {
        qCWarning(RUQOLA_LOG) << " Info is invalid ! " << contents;
    } else {
        switch (info.notificationType()) {
        case NotificationInfo::StandardMessage: {
            const QString iconFileName = mCache->avatarUrlFromCacheOnly(info.senderUserName());
            // qDebug() << " iconFileName" << iconFileName << " sender " << info.senderId() << " info.senderUserName() " << info.senderUserName();
            QPixmap pix;
            if (!iconFileName.isEmpty()) {
                const QUrl url = QUrl::fromLocalFile(iconFileName);
                // qDebug() << "url.toLocalFile()" << url.toLocalFile();
                const bool loaded = pix.load(url.toLocalFile().remove(QStringLiteral("file://")), "JPEG");
                // qDebug() << " load pixmap : " << loaded;
                // qDebug() << " pix " << pix.isNull();
                Q_UNUSED(loaded)
                info.setPixmap(pix);
            }
            break;
        }
        case NotificationInfo::ConferenceCall: {
            // Nothing
            break;
        }
        }

        Q_EMIT notification(info);
    }
}

void RocketChatAccount::inputAutocomplete(const QString &roomId,
                                          const QString &pattern,
                                          const QString &exceptions,
                                          InputTextManager::CompletionForType type,
                                          bool threadDialog)
{
    // TODO look at for restapi support.
    switch (type) {
    case InputTextManager::CompletionForType::Channel:
        ddp()->inputChannelAutocomplete(roomId, pattern, exceptions, threadDialog);
        break;
    case InputTextManager::CompletionForType::User:
        ddp()->inputUserAutocomplete(roomId, pattern, exceptions, threadDialog);
        break;
    default:
        break;
    }
}

AutotranslateLanguagesModel *RocketChatAccount::autoTranslateLanguagesModel() const
{
    return mAutoTranslateLanguagesModel;
}

void RocketChatAccount::updateUser(const QJsonObject &object)
{
    mUserModel->updateUser(object);
}

void RocketChatAccount::userStatusChanged(const User &user)
{
    if (user.userId() == userId()) {
        mPresenceStatus = user.status();
        statusModel()->setCurrentPresenceStatus(mPresenceStatus);
        statusModel()->setCustomText(user.statusText());
        Q_EMIT userStatusUpdated(mPresenceStatus, user.statusText(), accountName());
    }
    mUserModel->addUser(user);
    mRoomModel->userStatusChanged(user);
}

void RocketChatAccount::ignoreUser(const QString &rid, const QString &userId, bool ignore)
{
    restApi()->ignoreUser(rid, userId, ignore);
}

void RocketChatAccount::blockUser(const QString &rid, bool block)
{
    // TODO use restapi
    if (rid.isEmpty()) {
        qCWarning(RUQOLA_LOG) << " void RocketChatAccount::blockUser EMPTY rid ! block " << block;
    } else {
        // qDebug() << " void RocketChatAccount::blockUser userId " << userId << " block " << block << " rid " << rid << " own userdId" << userID();

        const QString userIdFromDirectChannel = Utils::userIdFromDirectChannel(rid, userId());
        if (block) {
            ddp()->blockUser(rid, userIdFromDirectChannel);
        } else {
            ddp()->unBlockUser(rid, userIdFromDirectChannel);
        }
    }
}

void RocketChatAccount::clearTypingNotification()
{
    mReceiveTypingNotificationManager->clearTypingNotification();
}

void RocketChatAccount::checkInitializedRoom(const QString &roomId)
{
    Room *r = mRoomModel->findRoom(roomId);
    if (r && !r->wasInitialized()) {
        r->setWasInitialized(true);
        ddp()->subscribeRoomMessage(roomId);
        if (!r->archived()) {
            membersInRoom(r->roomId(), r->channelType());
            rolesInRoom(r->roomId(), r->channelType());
            if (r->channelType() == Room::RoomType::Channel) {
                restApi()->getChannelsCounter(r->roomId());
            }
        }
        loadHistory(r->roomId(), true /*initial loading*/);
    } else if (!r) {
        qWarning() << " Room " << roomId << " was no found! Need to open it";
        // openDirectChannel(roomId);
    }
    QMetaObject::invokeMethod(this, &RocketChatAccount::switchedRooms, Qt::QueuedConnection);
}

void RocketChatAccount::openDocumentation()
{
    QDesktopServices::openUrl(QUrl(QStringLiteral("help:/")));
}

void RocketChatAccount::updateAvatarCache(const Utils::AvatarInfo &info)
{
    mCache->updateAvatar(info);
}

void RocketChatAccount::avatarChanged(const QJsonArray &contents)
{
    // qDebug() << " void RocketChatAccount::avatarChanged(const QJsonArray &contents)*******************" << contents;
    for (int i = 0; i < contents.count(); ++i) {
        const QJsonObject obj = contents.at(i).toObject();
        if (obj.contains(QLatin1String("username"))) {
            const QString userName = obj[QLatin1String("username")].toString();
            Utils::AvatarInfo info;
            info.avatarType = Utils::AvatarType::User;
            info.identifier = userName;
            Q_EMIT avatarWasChanged(info);
        } else if (obj.contains(QLatin1String("rid"))) {
            const QString roomId = obj[QLatin1String("rid")].toString();
            const QString etag = obj[QLatin1String("etag")].toString();
            qDebug() << "need to update room avatar " << accountName() << "room" << roomId << "etag " << etag;
            Utils::AvatarInfo info;
            info.avatarType = Utils::AvatarType::Room;
            info.etag = etag; // Etag
            info.identifier = roomId; // roomId
            Q_EMIT avatarWasChanged(info);
        } else {
            qWarning() << "avatar changed but missing roomId or userId. It seems to be a regression in RC? " << contents;
        }
    }

    // TODO parse "QJsonObject({"args":[{"username":"foo"}],"eventName":"updateAvatar"})"
}

void RocketChatAccount::rolesChanged(const QJsonArray &contents)
{
    // TODO verify this code when role change. It seems weird.
    for (int i = 0; i < contents.count(); ++i) {
        const QJsonObject obj = contents.at(i).toObject();
        const QString scope = obj[QLatin1String("scope")].toString();
        Room *room = mRoomModel->findRoom(scope);
        if (room) {
            room->updateRoles(obj);
        } else {
            qWarning() << " Impossible to find room associate to " << scope << contents;
        }
    }
}

void RocketChatAccount::createDiscussion(const QString &parentRoomId,
                                         const QString &discussionName,
                                         const QString &replyMessage,
                                         const QString &messageId,
                                         const QStringList &users)
{
    restApi()->createDiscussion(parentRoomId, discussionName, replyMessage, messageId, users);
}

void RocketChatAccount::threadsInRoom(const QString &roomId, bool onlyUnread)
{
    if (threadsEnabled()) {
        mListMessageModel->clear();
        mListMessageModel->setRoomId(roomId);
        restApi()->getThreadsList(roomId, onlyUnread);
    }
}

void RocketChatAccount::discussionsInRoom(const QString &roomId)
{
    mDiscussionsModel->initialize();
    mDiscussionsModel->setLoadMoreDiscussionsInProgress(true);
    restApi()->getDiscussions(roomId);
}

void RocketChatAccount::followMessage(const QString &messageId, bool follow)
{
    if (follow) {
        restApi()->followMessage(messageId);
    } else {
        restApi()->unFollowMessage(messageId);
    }
}

void RocketChatAccount::getSupportedLanguages()
{
    if (autoTranslateEnabled()) {
        bool needTargetLanguage = false;
        if (ruqolaServerConfig()->hasAtLeastVersion(5, 1, 0)) {
            needTargetLanguage = true;
        }
        auto job = new RocketChatRestApi::GetSupportedLanguagesJob(this);
        job->setNeedTargetLanguage(needTargetLanguage);
        restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::GetSupportedLanguagesJob::getSupportedLanguagesDone, this, &RocketChatAccount::slotGetSupportedLanguagesDone);
        if (!job->start()) {
            qCDebug(RUQOLA_LOG) << "Impossible to start getSupportedLanguagesMessages";
        }
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::getSupportedLanguages is not supported before server 2.0.0";
    }
}

void RocketChatAccount::slotGetSupportedLanguagesDone(const QJsonObject &obj)
{
    mAutoTranslateLanguagesModel->parseLanguages(obj);
}

void RocketChatAccount::autoTranslateSaveLanguageSettings(const QString &roomId, const QString &language)
{
    restApi()->autoTranslateSaveLanguageSettings(roomId, language);
}

void RocketChatAccount::autoTranslateSaveAutoTranslateSettings(const QString &roomId, bool autoTranslate)
{
    restApi()->autoTranslateSaveAutoTranslateSettings(roomId, autoTranslate);
}

void RocketChatAccount::slotUsersPresenceDone(const QJsonObject &obj)
{
    // qDebug() << " void RocketChatAccount::slotUsersPresenceDone(const QJsonObject &obj)" << obj;
    const auto lst = obj.value(QStringLiteral("users")).toArray();
    for (const auto &var : lst) {
        const QJsonObject userJson = var.toObject();
        User user;
        user.parseUserRestApi(userJson, roleInfo());
        if (user.isValid()) {
            userStatusChanged(user);
        }
    }
}

void RocketChatAccount::slotReconnectToServer()
{
    // This happens when we didn't react to pings for a while
    // (e.g. while stopped in gdb, or if network went down for a bit)
    // Let's try connecting in again
    QTimer::singleShot(mDelayReconnect, this, [this]() {
        qCDebug(RUQOLA_RECONNECT_LOG) << "Attempting to reconnect after the server disconnected us: " << accountName();
        if (mDelayReconnect == 100) {
            mDelayReconnect = 1000;
        } else {
            mDelayReconnect *= 2;
        }
        Q_EMIT displayReconnectWidget(mDelayReconnect / 1000);
        tryLogin();
    });
}

void RocketChatAccount::usersPresence()
{
    restApi()->usersPresence();
}

void RocketChatAccount::customUsersStatus()
{
    restApi()->customUserStatus();
}

void RocketChatAccount::initializeAccount()
{
    listEmojiCustom();

    // load when necessary
    usersPresence();
    // Force set online.
    // TODO don't reset message status !
    if (RuqolaGlobalConfig::self()->setOnlineAccounts()) {
        ddp()->setDefaultStatus(User::PresenceStatus::PresenceOnline);
    }
    // Initialize sounds
    mCustomSoundManager->initializeDefaultSounds();
    ddp()->listCustomSounds();
    customUsersStatus();
    slotLoadRoles();
    if (mRuqolaServerConfig->hasAtLeastVersion(5, 0, 0)) {
        checkLicenses();
    }

    Q_EMIT accountInitialized();
}

void RocketChatAccount::checkLicenses()
{
    auto job = new RocketChatRestApi::LicensesIsEnterpriseJob(this);
    restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::LicensesIsEnterpriseJob::licensesIsEnterpriseDone, this, [this](bool isEnterprise) {
        mRuqolaServerConfig->setHasEnterpriseSupport(isEnterprise);
        if (isEnterprise) {
            ddp()->licenseGetModules();
        }
    });
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start LicensesIsEnterpriseJob job";
    }
}

LocalDatabaseManager *RocketChatAccount::localDatabaseManager() const
{
    return mLocalDatabaseManager.get();
}

VideoConferenceMessageInfoManager *RocketChatAccount::videoConferenceMessageInfoManager() const
{
    return mVideoConferenceMessageInfoManager;
}

VideoConferenceManager *RocketChatAccount::videoConferenceManager() const
{
    return mVideoConferenceManager;
}

void RocketChatAccount::downloadAppsLanguages()
{
    mDownloadAppsLanguagesManager->setServerVersion(mServerConfigInfo->serverVersionStr());
    mDownloadAppsLanguagesManager->setAccountName(mSettings->accountName());
    mDownloadAppsLanguagesManager->parse(mSettings->serverUrl());
}

void RocketChatAccount::slotFileLanguagedParsed()
{
    // We need mDownloadAppsLanguagesManager result for updating command
    getListCommands();
}

void RocketChatAccount::getListCommands()
{
    auto job = new RocketChatRestApi::ListCommandsJob(this);
    restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::ListCommandsJob::listCommandsDone, this, &RocketChatAccount::slotListCommandDone);
    if (!job->start()) {
        qCDebug(RUQOLA_LOG) << "Impossible to start ListPermissionsJob job";
    }
}

void RocketChatAccount::slotListCommandDone(const QJsonObject &obj)
{
    Commands commands;
    commands.setDownloadManager(mDownloadAppsLanguagesManager);
    commands.parseCommands(obj, this);
    if (!mCommandsModel->commands().isEmpty()) { // Don't show command listview if we already have command (for example when we logout/login)
        QSignalBlocker blockSignal(mCommandsModel);
        mCommandsModel->setCommands(commands);
    } else {
        // Initialize it after loading otherwise we will see listview at startup
        mCommandsModel->setCommands(commands);
        mInputTextManager->setCommandModel(mCommandsModel);
        mInputThreadMessageTextManager->setCommandModel(mCommandsModel);
    }
    // qCDebug(RUQOLA_COMMANDS_LOG) << "accountname " << accountName() << "\nslotListCommandDone " << obj;
}

bool RocketChatAccount::runCommand(const QString &msg, const QString &roomId, const QString &tmid)
{
    const RocketChatRestApi::RunCommandJob::RunCommandInfo info = RocketChatRestApi::RunCommandJob::parseString(msg, roomId, tmid);
    if (info.isValid()) {
        runCommand(info);
        return true;
    }
    return false;
}

void RocketChatAccount::runCommand(const RocketChatRestApi::RunCommandJob::RunCommandInfo &runCommandInfo)
{
    restApi()->runCommand(runCommandInfo);
}

User RocketChatAccount::fullUserInfo(const QString &userName) const
{
    return mUserModel->fullUserInfo(userName);
}

void RocketChatAccount::markMessageAsUnReadFrom(const QString &messageId)
{
    restApi()->markMessageAsUnReadFrom(messageId);
}

void RocketChatAccount::markRoomAsUnRead(const QString &roomId)
{
    restApi()->markRoomAsUnRead(roomId);
}

void RocketChatAccount::loginStatusChangedSlot()
{
    if (loginStatus() == DDPAuthenticationManager::LoggedOut) {
        Q_EMIT logoutDone(accountName());
        qCDebug(RUQOLA_LOG) << "Successfully logged out!";
    } else if (loginStatus() == DDPAuthenticationManager::LoggedIn) {
        // Reset it.
        mDelayReconnect = 100;
    } else if (loginStatus() == DDPAuthenticationManager::LoginFailedInvalidUserOrPassword) {
        // clear auth token to refresh it with the next login
        setAuthToken({});
    }
    Q_EMIT loginStatusChanged();
}

void RocketChatAccount::sendUserEmailCode()
{
    restApi()->sendUserEmailCode(userName());
}

void RocketChatAccount::requestNewPassword(const QString &email)
{
    restApi()->forgotPassword(email);
}

void RocketChatAccount::registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo)
{
    restApi()->registerNewUser(userInfo);
}

void RocketChatAccount::deleteOwnAccount(const QString &password)
{
    restApi()->deleteOwnAccount(password);
}

bool RocketChatAccount::allowEmailChange() const
{
    return serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowEmailChange;
}

bool RocketChatAccount::allowPasswordChange() const
{
    return serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowPasswordChange;
}

bool RocketChatAccount::allowPasswordReset() const
{
    return serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowPasswordReset;
}

bool RocketChatAccount::allowUsernameChange() const
{
    return serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowUsernameChange;
}

void RocketChatAccount::slotRegisterUserDone()
{
    Q_EMIT registerUserSuccess();
}

void RocketChatAccount::updateOwnBasicInfo(const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &info)
{
    restApi()->updateOwnBasicInfo(info);
}

RuqolaServerConfig::ServerConfigFeatureTypes RocketChatAccount::serverConfigFeatureTypes() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes();
}

void RocketChatAccount::parseOwnInfoDone(const QJsonObject &replyObject)
{
    mOwnUser.parseOwnUserInfo(replyObject);
    mAwayManager->updateSettings();
    mBannerInfos.parseBannerInfos(replyObject);
    const User user = mOwnUser.user();
    // qDebug() << " USER  " << user;
    if (user.isValid()) {
        usersModel()->addUser(user);
        if (!RuqolaGlobalConfig::self()->setOnlineAccounts()) {
            // Need to update own status.
            setOwnStatus(user);
        }
    } else {
        qCWarning(RUQOLA_LOG) << " Error during parsing user" << replyObject;
    }
    downloadAppsLanguages();
    Q_EMIT bannerInfoChanged();
    Q_EMIT ownInfoChanged();
    Q_EMIT ownUserPreferencesChanged();
}

bool RocketChatAccount::isAdministrator() const
{
    return mOwnUser.isAdministrator();
}

void RocketChatAccount::slotChannelGetCountersDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    Room *room = mRoomModel->findRoom(channelInfo.identifier);
    if (room) {
        ChannelCounterInfo info;
        info.parseCounterInfo(obj);
        room->setChannelCounterInfo(info);
    }
}

void RocketChatAccount::slotMarkAsReadDone(const QString &roomId)
{
    Room *room = this->room(roomId);
    if (room) {
        room->setChannelCounterInfo({});
    }
}

void RocketChatAccount::logoutFromOtherLocation()
{
    restApi()->removeOtherTokens();
}

void RocketChatAccount::createDirectMessages(const QStringList &usernames)
{
    restApi()->createDirectMessage(usernames);
}

void RocketChatAccount::slotCustomUserStatusDone(const QJsonObject &customList)
{
    mCustomUserStatuses.parseCustomUserStatuses(customList);
    // qDebug() << "customList  " << mCustomUserStatuses;
    Q_EMIT customUserStatusChanged();
}

CustomUserStatuses RocketChatAccount::customUserStatuses() const
{
    return mCustomUserStatuses;
}

void RocketChatAccount::removeCustomUserStatus(const QString &customUserStatusId)
{
    restApi()->deleteCustomUserStatus(customUserStatusId);
}

void RocketChatAccount::updateCustomUserStatus(const RocketChatRestApi::CustomUserStatusUpdateJob::StatusUpdateInfo &statusUpdateInfo)
{
    restApi()->updateCustomUserStatus(statusUpdateInfo);
}

void RocketChatAccount::createCustomUserStatus(const RocketChatRestApi::CustomUserStatusCreateJob::StatusCreateInfo &statusCreateInfo)
{
    restApi()->createCustomUserStatus(statusCreateInfo);
}

void RocketChatAccount::slotPostMessageDone(const QJsonObject &replyObject)
{
    // qDebug() << "replyObject " << replyObject;
    const QJsonObject roomInfo = replyObject[QLatin1String("message")].toObject();
    addMessage(roomInfo, true, true);
}

void RocketChatAccount::updateUserData(const QJsonArray &contents)
{
    qDebug() << " void RocketChatAccount::updateUserData(const QJsonArray &contents)" << contents;
    for (const auto &array : contents) {
        const QJsonObject updateJson = array[QLatin1String("diff")].toObject();
        const QStringList keys = updateJson.keys();
        OwnUserPreferences ownUserPreferences = mOwnUser.ownUserPreferences();
        for (const QString &key : keys) {
            if (key == QLatin1String("settings.preferences.highlights")) {
                const QJsonArray highlightsArray = updateJson.value(key).toArray();
                ownUserPreferences.updateHighlightWords(highlightsArray);
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == QLatin1String("settings.preferences.enableAutoAway")) {
                ownUserPreferences.setEnableAutoAway(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
            } else if (key == QLatin1String("settings.preferences.convertAsciiEmoji")) {
                ownUserPreferences.setConvertAsciiEmoji(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == QLatin1String("settings.preferences.hideRoles")) {
                ownUserPreferences.setHideRoles(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == QLatin1String("settings.preferences.displayAvatars")) {
                ownUserPreferences.setDisplayAvatars(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == QLatin1String("settings.preferences.messageViewMode")) {
                ownUserPreferences.setMessageViewMode(updateJson.value(key).toInt());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == QLatin1String("settings.preferences.sidebarViewMode")) { // Channel List view mode
                // TODO
            } else if (key == QLatin1String("settings.preferences.sidebarShowUnread")) {
                ownUserPreferences.setShowUnread(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == QLatin1String("settings.preferences.sidebarDisplayAvatar")) { // Avatar in channel list view
                ownUserPreferences.setShowRoomAvatar(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == QLatin1String("settings.preferences.sidebarShowFavorites")) {
                ownUserPreferences.setShowFavorite(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == QLatin1String("settings.preferences.sidebarSortby")) {
                const QString value = updateJson.value(key).toString();
                if (value == QLatin1String("activity")) {
                    ownUserPreferences.setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::ByLastMessage);
                } else if (value == QLatin1String("alphabetical")) {
                    ownUserPreferences.setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::Alphabetically);
                } else {
                    qCWarning(RUQOLA_LOG) << "Sortby is not defined ?  " << value;
                }
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else {
                const static QRegularExpression bannerRegularExpression(QStringLiteral("banners.(.*).read"));
                QRegularExpressionMatch rmatch;
                if (key.contains(bannerRegularExpression, &rmatch)) {
                    if (rmatch.hasMatch()) {
                        const QString bannerName = rmatch.captured(1);
                        const bool result = updateJson.value(key).toBool();
                        mBannerInfos.updateBannerReadInfo(bannerName, result);
                    }
                }
            }
        }
    }
    // QJsonArray([{"diff":{"_updatedAt":{"$date":1639552419120},"avatarETag":"MCGFkLtBKkhb5GXBj","avatarOrigin":"rest"},"type":"updated","unset":{}}])
    // QJsonArray([{"diff":{"_updatedAt":{"$date":1639552237550}},"type":"updated","unset":{"avatarETag":1,"avatarOrigin":1}}])
    // QJsonArray([{"diff":{"_updatedAt":{"$date":1639552298748},"nickname":"ss"},"type":"updated","unset":{}}])
    // QJsonArray([{"diff":{"_updatedAt":{"$date":1639552390152}},"type":"updated","unset":{"nickname":1}}])
    // TODO
}

void RocketChatAccount::addMessage(const QJsonObject &replyObject, bool useRestApi, bool temporaryMessage)
{
    const QString roomId = replyObject.value(QLatin1String("rid")).toString();
    if (!roomId.isEmpty()) {
        MessagesModel *messageModel = messageModelForRoom(roomId);
        if (!messageModel) {
            qCWarning(RUQOLA_LOG) << "Unexpected null message model.";
            return;
        }
        Message m(emojiManager());
        m.parseMessage(replyObject, useRestApi);
        m.setMessageType(Message::MessageType::Information);
        m.setPendingMessage(temporaryMessage);
        if (!m.threadMessageId().isEmpty()) {
            // qDebug() << " It's a thread message id ****************************" << m.threadMessageId();
            updateThreadMessageList(m);
        }
        // m.setMessageType(Message::System);
        // TODO add special element!See roomData QJsonObject({"_id":"u9xnnzaBQoQithsxP","msg":"You have been muted and cannot speak in this
        // room","rid":"Dic5wZD4Zu9ze5gk3","ts":{"$date":1534166745895}})
        // Temporary => we don't add it in database
        messageModel->addMessages({m});
    } else {
        qCWarning(RUQOLA_LOG) << "stream-notify-user : Message: ROOMID is empty ";
    }
}

OwnUserPreferences RocketChatAccount::ownUserPreferences() const
{
    return ownUser().ownUserPreferences();
}

QStringList RocketChatAccount::highlightWords() const
{
    return ownUser().ownUserPreferences().highlightWords();
}

void RocketChatAccount::resetAvatar()
{
    RocketChatRestApi::UserBaseJob::UserInfo info;
    info.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
    info.userIdentifier = userId();
    restApi()->resetAvatar(info);
    // TODO update avatar when we reset it.
}

void RocketChatAccount::setAvatarUrl(const QString &url)
{
    RocketChatRestApi::UserBaseJob::UserInfo userInfo;
    userInfo.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
    userInfo.userIdentifier = userId();
    RocketChatRestApi::SetAvatarJob::SetAvatarInfo avatarInfo;
    avatarInfo.mAvatarUrl = url;
    restApi()->setAvatar(userInfo, avatarInfo);
}

void RocketChatAccount::setImageUrl(const QUrl &url)
{
    RocketChatRestApi::UserBaseJob::UserInfo userInfo;
    userInfo.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
    userInfo.userIdentifier = userId();
    RocketChatRestApi::SetAvatarJob::SetAvatarInfo avatarInfo;
    avatarInfo.mImageUrl = url;
    restApi()->setAvatar(userInfo, avatarInfo);
}

void RocketChatAccount::exportMessages(const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &info)
{
    auto job = new RocketChatRestApi::RoomsExportJob(this);
    job->setRoomExportInfo(info);
    restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::RoomsExportJob::roomExportDone, this, &RocketChatAccount::slotRoomExportDone);
    if (!job->start()) {
        qCDebug(RUQOLA_LOG) << "Impossible to start RoomsExportJob";
    }
}

void RocketChatAccount::slotRoomExportDone()
{
    auto notification = new KNotification(QStringLiteral("export-message"), KNotification::CloseOnTimeout);
    notification->setTitle(i18n("Export Messages"));
    notification->setText(i18n("Your email has been queued for sending."));
    notification->sendEvent();
}

void RocketChatAccount::slotPermissionListAllDone(const QJsonObject &replyObject)
{
    // qDebug() << accountName() << " replyObject " << replyObject;
    mPermissionManager.parsePermissions(replyObject);
    Q_EMIT permissionChanged();
    // We can't load until permission loaded.
    if (hasAutotranslateSupport()) {
        getSupportedLanguages();
    }
}

QStringList RocketChatAccount::permissions(const QString &permissionId) const
{
    return mPermissionManager.roles(permissionId);
}

QStringList RocketChatAccount::ownUserPermission() const
{
    return mOwnUser.roles();
}

bool RocketChatAccount::hasPermission(const QString &permissionId) const
{
    const QStringList ownUserRoles{mOwnUser.roles()};
    const QStringList permissionRoles{mPermissionManager.roles(permissionId)};
    if (permissionRoles.isEmpty()) { // Check if we don't have stored permissionId in permission manager
        if (!mPermissionManager.contains(permissionId)) {
            qCWarning(RUQOLA_LOG) << "permissionId not loaded during setup:" << permissionId;
        }
    }
    for (const QString &role : permissionRoles) {
        if (ownUserRoles.contains(role)) {
            return true;
        }
    }
    return false;
}

void RocketChatAccount::setUserPreferences(const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &info)
{
    restApi()->setUserPreferences(info);
}

void RocketChatAccount::slotUsersSetPreferencesDone(const QJsonObject &replyObject)
{
    // qDebug() << " void RocketChatAccount::slotUsersSetPreferencesDone(const QJsonObject &replyObject)" << replyObject;
    const QJsonObject user = replyObject.value(QLatin1String("user")).toObject();
    if (user.value(QLatin1String("_id")).toString() == userId()) {
        OwnUserPreferences ownUserPreferences;
        ownUserPreferences.parsePreferences(user.value(QLatin1String("settings")).toObject().value(QLatin1String("preferences")).toObject());
        mOwnUser.setOwnUserPreferences(ownUserPreferences);
        Q_EMIT ownUserPreferencesChanged();
    }
}

bool RocketChatAccount::hasAutotranslateSupport() const
{
    return autoTranslateEnabled() && hasPermission(QStringLiteral("auto-translate"));
}

MessageCache *RocketChatAccount::messageCache() const
{
    return mMessageCache;
}

void RocketChatAccount::slotUpdateCustomUserStatus()
{
    mStatusModel->updateCustomStatus(mCustomUserStatuses.customUserses());
    Q_EMIT customStatusChanged();
}

bool RocketChatAccount::hideRoles() const
{
    return ownUser().ownUserPreferences().hideRoles();
}

bool RocketChatAccount::displayAvatars() const
{
    return ownUser().ownUserPreferences().displayAvatars();
}

int RocketChatAccount::messageViewMode() const
{
    return ownUser().ownUserPreferences().messageViewMode();
}

void RocketChatAccount::slotLoadRoles()
{
    // First load list of roles.
    auto job = new RocketChatRestApi::RolesListJob(this);
    restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::RolesListJob::rolesListDone, &mRolesManager, &RolesManager::parseRoles);
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start RolesListJob job";
    }
}

CustomSoundsManager *RocketChatAccount::customSoundManager() const
{
    return mCustomSoundManager;
}

void RocketChatAccount::slotAwayStatusChanged(bool away)
{
    restApi()->setUserStatus(userId(), away ? RocketChatRestApi::SetStatusJob::Away : RocketChatRestApi::SetStatusJob::OnLine, {});
    qCDebug(RUQOLA_LOG) << "RocketChatAccount::slotAwayStatusChanged  " << away;
}

void RocketChatAccount::generate2FaTotp(const QJsonObject &obj)
{
    // qDebug() << "RocketChatAccount::generate2FaTotp " << obj;
    const QString secret = obj.value(QStringLiteral("secret")).toString();
    const QString url = obj.value(QStringLiteral("url")).toString();
    Q_EMIT totpResult(secret, url);
}

void RocketChatAccount::totpDisabledVerify(const QJsonObject &root)
{
    const int result = root.value(QStringLiteral("result")).toInt();
    Q_EMIT disabledTotpValid(result == 1);
}

void RocketChatAccount::totpVerify(const QJsonObject &obj)
{
    if (obj.isEmpty()) {
        Q_EMIT totpInvalid();
    } else {
        // qDebug() << "totpValid " << obj;
        QStringList lstCodes;
        const QJsonArray codes = obj.value(QStringLiteral("codes")).toArray();
        const auto nbCodes{codes.count()};
        lstCodes.reserve(nbCodes);
        for (auto i = 0; i < nbCodes; ++i) {
            lstCodes.append(codes.at(i).toString());
        }
        Q_EMIT totpValid(lstCodes);
    }
}

void RocketChatAccount::setOauthAppAdded(const QJsonObject &obj)
{
    // TODO return error
    Q_EMIT oauthAppAdded(obj);
}

void RocketChatAccount::setOauthAppUpdated(const QJsonObject &obj)
{
    // TODO return error
    Q_EMIT oauthAppUpdated(obj);
}

bool RocketChatAccount::hasLicense(const QString &name)
{
    return mLicensesManager.hasLicense(name);
}

void RocketChatAccount::parseLicenses(const QJsonArray &replyArray)
{
    mLicensesManager.parseLicenses(replyArray);
}

void RocketChatAccount::addMessageToDataBase(const QString &roomName, const Message &message)
{
    mLocalDatabaseManager->addMessage(accountName(), roomName, message);
}

void RocketChatAccount::deleteMessageFromDatabase(const QString &roomName, const QString &messageId)
{
    mLocalDatabaseManager->deleteMessage(accountName(), roomName, messageId);
}

// Only for debugging permissions. (debug mode)
QVector<Permission> RocketChatAccount::permissions() const
{
    return mPermissionManager.permissions();
}

#include "moc_rocketchataccount.cpp"
