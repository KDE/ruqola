/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccount.h"
#include "accountroomsettings.h"
#include "apps/appcountjob.h"
#include "apps/appinstalledjob.h"
#include "apps/appscountinfo.h"
#include "apps/appsmarketplaceinstalledinfo.h"
#include "config-ruqola.h"
#include "memorymanager/memorymanager.h"
#include "misc/methodcalljob.h"
#include "model/appscategoriesmodel.h"
#include "model/roommodel.h"
#include "notifications/notificationpreferences.h"
#include "rocketchataccountsettings.h"
#include "ruqolautils.h"

#include "attachments/fileattachments.h"
#include "authenticationmanager.h"
#include "authenticationmanager/ddpauthenticationmanager.h"
#include "autotranslate/getsupportedlanguagesjob.h"
#include "commands/listcommandsjob.h"
#include "commands/runcommandjob.h"
#include "customemojiiconmanager.h"
#include "downloadappslanguages/downloadappslanguagesmanager.h"
#include "emoticons/emojimanager.h"
#include "encryption/e2ekeymanager.h"
#include "managerdatapaths.h"
#include "messagequeue.h"
#include "previewurlcachemanager.h"
#include "serverconfiginfo.h"
#include "soundmanager.h"

#include "authenticationmanager/restauthenticationmanager.h"
#include "model/appsmarketplacemodel.h"
#include "model/autotranslatelanguagesmodel.h"
#include "model/commandsmodel.h"
#include "model/commonmessagesmodel.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/discussionsmodel.h"
#include "model/emoticonmodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include "model/listmessagesfilterproxymodel.h"
#include "model/messagesmodel.h"
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
#include "ruqola_sound_debug.h"
#include "ruqola_typing_notification_debug.h"
#include "ruqolaglobalconfig.h"
#include "ruqolalogger.h"
#include "typingnotification.h"
#include <TextEmoticonsCore/UnicodeEmoticonManager>

#include "channelcounterinfo.h"
#include "connection.h"
#include "directmessage/opendmjob.h"
#include "discussions/discussions.h"
#include "emoji/loademojicustomjob.h"
#include "license/licensesinfojob.h"
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
#include <QJsonArray>
#include <QTimer>
#include <TextEmoticonsCore/EmojiModel>
#include <TextEmoticonsCore/EmojiModelManager>

#if HAVE_NETWORKMANAGER
#include <NetworkManagerQt/Manager>
#endif

#include "plugins/pluginauthentication.h"
#include "plugins/pluginauthenticationinterface.h"

#include "apps/appcategoriesjob.h"
#include "apps/appmarketplacejob.h"
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
    , mPreviewUrlCacheManager(new PreviewUrlCacheManager(this, this))
    , mNotificationPreferences(new NotificationPreferences(this))
    , mE2eKeyManager(new E2eKeyManager(this, this))
    , mSoundManager(new SoundManager(this))
    , mAppsMarketPlaceModel(new AppsMarketPlaceModel(this))
    , mAppsCategoriesModel(new AppsCategoriesModel(this))
    , mMemoryManager(new MemoryManager(this))
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
            [this](const QByteArray &roomId, const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type) {
                inputAutocomplete(roomId, pattern, exceptions, type, false);
            });

    mInputThreadMessageTextManager->setObjectName(QStringLiteral("mInputThreadMessageTextManager"));
    connect(mInputThreadMessageTextManager,
            &InputTextManager::completionRequested,
            this,
            [this](const QByteArray &roomId, const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type) {
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

    mFilesModelForRoom = new FilesForRoomModel(this, this);
    mFilesModelForRoom->setObjectName(QStringLiteral("filesmodelforrooms"));
    mFilesForRoomFilterProxyModel = new FilesForRoomFilterProxyModel(mFilesModelForRoom, this);
    mFilesForRoomFilterProxyModel->setObjectName(QStringLiteral("filesforroomfiltermodelproxy"));

    mDiscussionsModel->setObjectName(QStringLiteral("discussionsmodel"));
    mDiscussionsFilterProxyModel = new DiscussionsFilterProxyModel(mDiscussionsModel, this);
    mDiscussionsFilterProxyModel->setObjectName(QStringLiteral("discussionsfilterproxymodel"));

    mThreadMessageModel = new ThreadMessageModel(this, this);
    mThreadMessageModel->setObjectName(QStringLiteral("threadmessagemodel"));

    mListMessageModel = new ListMessagesModel(QByteArray(), this, nullptr, this);
    mListMessageModel->setObjectName(QStringLiteral("listmessagemodel"));

    mListMessagesFilterProxyModel = new ListMessagesFilterProxyModel(mListMessageModel, this);
    mListMessagesFilterProxyModel->setObjectName(QStringLiteral("listmessagesfiltermodelproxy"));

    mAutoTranslateLanguagesModel->setObjectName(QStringLiteral("autotranslatelanguagesmodel"));

    connect(mRoomModel, &RoomModel::needToUpdateNotification, this, &RocketChatAccount::slotNeedToUpdateNotification);
    connect(mRoomModel, &RoomModel::roomNeedAttention, this, &RocketChatAccount::slotRoomNeedAttention);
    connect(mRoomModel, &RoomModel::roomRemoved, this, &RocketChatAccount::roomRemoved);
    connect(mRoomModel, &RoomModel::openChanged, this, &RocketChatAccount::slotRoomOpenChanged);

    mMessageQueue = new MessageQueue(this, this);
    mTypingNotification = new TypingNotification(this);
    mCache = new RocketChatCache(this, this);

    connect(mDownloadAppsLanguagesManager, &DownloadAppsLanguagesManager::fileLanguagesParseSuccess, this, &RocketChatAccount::slotFileLanguagedParsed);
    connect(mDownloadAppsLanguagesManager, &DownloadAppsLanguagesManager::fileLanguagesParseFailed, this, &RocketChatAccount::slotFileLanguagedParsed);

    connect(mCache, &RocketChatCache::fileDownloaded, this, &RocketChatAccount::fileDownloaded);
    connect(mTypingNotification, &TypingNotification::informTypingStatus, this, &RocketChatAccount::slotInformTypingStatus);
    connect(this, &RocketChatAccount::customUserStatusChanged, this, &RocketChatAccount::slotUpdateCustomUserStatus);
    QTimer::singleShot(0, this, &RocketChatAccount::clearModels);

#if HAVE_NETWORKMANAGER
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::primaryConnectionChanged, this, [this](const QString &uni) {
        // If there is a new network connection, disconnect and reconnect/login.
        // The uni is "/" when the last primary connection was closed.
        // Do not log out to keep the messages visible (and because we can't, without a connected socket).
        // TODO Login only if we were logged in at this point.
        if (uni != "/"_L1 && mDdp) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "Disconnect and reconnect:" << accountName();
            forceDisconnect();
            reconnectToServer();
        }
    });
#endif
    connect(mManageChannels, &ManageChannels::selectRoomByRoomIdRequested, this, &RocketChatAccount::selectRoomByRoomIdRequested);
    connect(mManageChannels, &ManageChannels::selectRoomByRoomNameRequested, this, &RocketChatAccount::selectRoomByRoomNameRequested);
    connect(mManageChannels, &ManageChannels::missingChannelPassword, this, &RocketChatAccount::missingChannelPassword);
    connect(mManageChannels, &ManageChannels::openArchivedRoom, this, &RocketChatAccount::openArchivedRoom);
    connect(&mRolesManager, &RolesManager::rolesChanged, this, &RocketChatAccount::rolesUpdated);
    connect(mCustomSoundManager, &CustomSoundsManager::customSoundRemoved, this, [this](const QByteArray &identifier, const QString &soundFilePath) {
        const QUrl url = soundUrlFromLocalCache(soundFilePath);
        QFile f(url.toLocalFile());
        if (!f.remove()) {
            qCWarning(RUQOLA_SOUND_LOG) << "Impossible to remove " << soundFilePath;
        }
        Q_EMIT customSoundRemoved(identifier);
    });
    connect(mCustomSoundManager, &CustomSoundsManager::customSoundAdded, this, [this](const QByteArray &identifier) {
        const QString urlFilePath = mCustomSoundManager->soundFilePath(identifier);
        if (!urlFilePath.isEmpty()) {
            (void)mCache->soundUrlFromLocalCache(urlFilePath);
            Q_EMIT customSoundAdded(identifier);
        }
    });
    connect(mCustomSoundManager, &CustomSoundsManager::customSoundUpdated, this, [this](const QByteArray &identifier) {
        const QString urlFilePath = mCustomSoundManager->soundFilePath(identifier);
        if (!urlFilePath.isEmpty()) {
            (void)mCache->soundUrlFromLocalCache(urlFilePath);
            // TODO update sound file
            Q_EMIT customSoundUpdated(identifier);
        }
    });
    connect(mAwayManager, &AwayManager::awayChanged, this, &RocketChatAccount::slotAwayStatusChanged);
    connect(mCustomSoundManager, &CustomSoundsManager::customSoundChanged, this, &RocketChatAccount::loadSoundFiles);

    mPreviewUrlCacheManager->setCachePath(ManagerDataPaths::self()->path(ManagerDataPaths::PreviewUrl, accountName()));
    setDefaultAuthentication(mSettings->authMethodType());
    mNotificationPreferences->setCustomSoundManager(mCustomSoundManager);
    connect(mE2eKeyManager, &E2eKeyManager::verifyKeyDone, this, &RocketChatAccount::slotVerifyKeysDone);
    connect(mMemoryManager, &MemoryManager::clearApplicationSettingsModelRequested, mAppsMarketPlaceModel, &AppsMarketPlaceModel::clear);
    connect(mMemoryManager, &MemoryManager::cleanRoomHistoryRequested, this, &RocketChatAccount::slotCleanRoomHistory);

    // Initiate socket connections, once we're out of Ruqola::self()
    if (accountEnabled()) {
        QMetaObject::invokeMethod(this, &RocketChatAccount::startConnecting, Qt::QueuedConnection);
    }
}

RocketChatAccount::~RocketChatAccount()
{
    delete mCache;
    mCache = nullptr;

    delete mRuqolaServerConfig;
    delete mRuqolaLogger;
    delete mAccountRoomSettings;
}

void RocketChatAccount::loadSoundFiles()
{
    const QList<CustomSoundInfo> customSoundInfos = mCustomSoundManager->customSoundsInfo();
    for (const CustomSoundInfo &info : customSoundInfos) {
        (void)soundUrlFromLocalCache(mCustomSoundManager->soundFilePath(info.identifier()));
    }
}

void RocketChatAccount::forceDisconnect()
{
    qCDebug(RUQOLA_RECONNECT_LOG) << "forcefully disconnecting" << accountName();
    mSettings->logout();
    mRoomModel->clear();
    mRestApi.reset();
    mDdp.reset();
}

void RocketChatAccount::reconnectToServer()
{
    qCDebug(RUQOLA_RECONNECT_LOG) << "reconnecting" << accountName();
    // Clear auth token otherwise we can't reconnect.
    mSettings->setAuthToken({});
    ddp();
    if (accountEnabled()) {
        Q_ASSERT(mDdp);
    }
    tryLogin();
}

Room::TeamRoomInfo RocketChatAccount::roomFromTeamId(const QByteArray &teamId) const
{
    return mRoomModel->roomFromTeamId(teamId);
}

void RocketChatAccount::removeSettings()
{
    mSettings->removeSettings();
    mCache->removeCache();
}

void RocketChatAccount::loadSettings(const QString &accountFileName)
{
    delete mSettings;
    mSettings = new RocketChatAccountSettings(accountFileName, this);
    if (mSettings->isValid()) {
        connect(mSettings, &RocketChatAccountSettings::serverURLChanged, this, [this]() {
            ddp()->onServerURLChange();
        });
        connect(mSettings, &RocketChatAccountSettings::userIdChanged, this, &RocketChatAccount::userIdChanged);
        connect(mSettings, &RocketChatAccountSettings::passwordAvailable, this, &RocketChatAccount::passwordAvailable);
        connect(mSettings, &RocketChatAccountSettings::activitiesChanged, this, &RocketChatAccount::activitiesChanged);
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

void RocketChatAccount::slotNeedToUpdateNotification()
{
    bool hasAlert = false;
    int nbUnread = 0;
    bool hasMention = false;
    mRoomModel->getUnreadAlertFromAccount(hasAlert, nbUnread, hasMention);
    Q_EMIT updateNotification(hasAlert, nbUnread, accountName());
}

void RocketChatAccount::clearModels()
{
    qCDebug(RUQOLA_RECONNECT_LOG) << "clear room model for" << accountName();
    // Clear rooms data and refill it with data in the cache, if there is
    mRoomModel->clear();

    mMessageQueue->loadCache();
    // Try to send queue message
    mMessageQueue->processQueue();
}

UserCompleterModel *RocketChatAccount::userCompleterModel() const
{
    return mUserCompleterModel;
}

UserCompleterFilterProxyModel *RocketChatAccount::userCompleterFilterProxyModel() const
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

UsersForRoomModel *RocketChatAccount::usersModelForRoom(const QByteArray &roomId) const
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

void RocketChatAccount::slotInformTypingStatus(const QByteArray &room, bool typing)
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

Room *RocketChatAccount::room(const QByteArray &roomId) const
{
    return mRoomModel->findRoom(roomId);
}

DiscussionsFilterProxyModel *RocketChatAccount::discussionsFilterProxyModel() const
{
    return mDiscussionsFilterProxyModel;
}

MessagesModel *RocketChatAccount::messageModelForRoom(const QByteArray &roomID)
{
    return mRoomModel->messageModel(roomID);
}

void RocketChatAccount::textEditing(const QByteArray &roomId, bool clearNotification)
{
    mTypingNotification->textNotificationChanged(roomId, clearNotification);
}

void RocketChatAccount::reactOnMessage(const QByteArray &messageId, const QString &emoji, bool shouldReact)
{
    if (emoji.startsWith(QLatin1Char(':')) && emoji.endsWith(QLatin1Char(':'))) {
        restApi()->reactOnMessage(messageId, emoji, shouldReact);
    } else {
        restApi()->reactOnMessage(messageId, mEmojiManager->normalizedReactionEmoji(emoji), shouldReact);
    }
}

void RocketChatAccount::sendMessage(const QByteArray &roomID, const QString &message)
{
    restApi()->postMessage(roomID, message);
    markRoomAsRead(roomID);
}

void RocketChatAccount::updateMessage(const QByteArray &roomID, const QByteArray &messageId, const QString &message)
{
    restApi()->updateMessage(roomID, messageId, message);
}

void RocketChatAccount::replyOnThread(const QByteArray &roomID, const QByteArray &threadMessageId, const QString &message)
{
    restApi()->sendMessage(roomID, message, QString(), threadMessageId);
}

QString RocketChatAccount::avatarUrl(const Utils::AvatarInfo &info)
{
    return mCache->avatarUrl(info);
}

void RocketChatAccount::insertAvatarUrl(const QString &userId, const QUrl &url)
{
    mCache->insertAvatarUrl(userId, url);
}

Connection *RocketChatAccount::restApi()
{
    if (!mRestApi) {
        mRestApi.reset(new Connection(this));

        connect(mRestApi.get(), &Connection::loginStatusChanged, this, &RocketChatAccount::slotRESTLoginStatusChanged);

        connect(mRestApi.get(), &Connection::channelMembersDone, this, &RocketChatAccount::parseUsersForRooms);
        connect(mRestApi.get(), &Connection::channelFilesDone, this, &RocketChatAccount::slotChannelFilesDone);
        connect(mRestApi.get(), &Connection::channelRolesDone, this, &RocketChatAccount::slotChannelGroupRolesDone);
        connect(mRestApi.get(), &Connection::groupRolesDone, this, &RocketChatAccount::slotChannelGroupRolesDone);
        connect(mRestApi.get(), &Connection::failed, this, &RocketChatAccount::slotJobFailed);
        connect(mRestApi.get(), &Connection::getThreadMessagesDone, this, &RocketChatAccount::slotGetThreadMessagesDone);
        connect(mRestApi.get(), &Connection::getDiscussionsDone, this, &RocketChatAccount::slotGetDiscussionsListDone);
        connect(mRestApi.get(), &Connection::markAsReadDone, this, &RocketChatAccount::slotMarkAsReadDone);
        connect(mRestApi.get(), &Connection::postMessageDone, this, &RocketChatAccount::slotPostMessageDone);
        connect(mRestApi.get(), &Connection::createChannelDone, this, &RocketChatAccount::slotCreateChannelDone);
        connect(mRestApi.get(), &Connection::createGroupDone, this, &RocketChatAccount::slotCreateGroupDone);
        connect(mRestApi.get(), &Connection::updateMessageFailed, this, &RocketChatAccount::updateMessageFailed);

        connect(mRestApi.get(), &Connection::getThreadsDone, this, [this](const QJsonObject &obj, const QString &roomId, bool onlyUnread) {
            slotGetListMessagesDone(obj,
                                    roomId.toLatin1(),
                                    onlyUnread ? ListMessagesModel::ListMessageType::UnreadThreadsMessages
                                               : ListMessagesModel::ListMessageType::ThreadsMessages);
        });
        connect(mRestApi.get(), &Connection::getMentionedMessagesDone, this, [this](const QJsonObject &obj, const QByteArray &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::MentionsMessages);
        });
        connect(mRestApi.get(), &Connection::getPinnedMessagesDone, this, [this](const QJsonObject &obj, const QByteArray &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::PinnedMessages);
        });
        connect(mRestApi.get(), &Connection::getStarredMessagesDone, this, [this](const QJsonObject &obj, const QByteArray &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::StarredMessages);
        });

        connect(mRestApi.get(), &Connection::usersPresenceDone, this, &RocketChatAccount::slotUsersPresenceDone);
        connect(mRestApi.get(), &Connection::usersAutocompleteDone, this, &RocketChatAccount::slotUserAutoCompleterDone);
        connect(mRestApi.get(), &Connection::registerUserDone, this, &RocketChatAccount::slotRegisterUserDone);
        connect(mRestApi.get(), &Connection::channelGetCountersDone, this, &RocketChatAccount::slotChannelGetCountersDone);
        connect(mRestApi.get(), &Connection::customUserStatusDone, this, &RocketChatAccount::slotCustomUserStatusDone);
        connect(mRestApi.get(), &Connection::permissionListAllDone, this, &RocketChatAccount::slotPermissionListAllDone);
        connect(mRestApi.get(), &Connection::usersSetPreferencesDone, this, &RocketChatAccount::slotUsersSetPreferencesDone);
        connect(mRestApi.get(), &Connection::networkError, this, [this]() {
            // Transient error, try again, with an increasing delay
            qCDebug(RUQOLA_RECONNECT_LOG) << "networkError" << accountName();
            forceDisconnect();
            autoReconnectDelayed();
        });

        mRestApi->setServerUrl(mSettings->serverUrl());
        mRestApi->setRestApiLogger(mRuqolaLogger);
        mCache->setRestApiConnection(mRestApi.get());
    }
    return mRestApi.get();
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

bool RocketChatAccount::previewEmbed() const
{
    return mRuqolaServerConfig->previewEmbed();
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

void RocketChatAccount::leaveRoom(const QByteArray &identifier, Room::RoomType channelType)
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

DDPClient *RocketChatAccount::ddp()
{
    if (!mDdp && accountEnabled()) {
        qCDebug(RUQOLA_RECONNECT_LOG) << "creating new DDPClient" << accountName();
        mDdp.reset(new DDPClient(this));
        if (Ruqola::useRestApiLogin()) {
            connect(mDdp->authenticationManager(), &DDPAuthenticationManager::loginStatusChanged, this, &RocketChatAccount::slotDDpLoginStatusChanged);
        } else {
            connect(mDdp->authenticationManager(), &DDPAuthenticationManager::loginStatusChanged, this, &RocketChatAccount::slotLoginStatusChanged);
        }
        connect(mDdp.get(), &DDPClient::connectedChanged, this, &RocketChatAccount::ddpConnectedChanged);
        connect(mDdp.get(), &DDPClient::changed, this, &RocketChatAccount::changed);
        connect(mDdp.get(), &DDPClient::added, this, &RocketChatAccount::added);
        connect(mDdp.get(), &DDPClient::removed, this, &RocketChatAccount::removed);
        connect(mDdp.get(), &DDPClient::socketError, this, &RocketChatAccount::socketError);
        connect(mDdp.get(), &DDPClient::disconnectedByServer, this, &RocketChatAccount::slotReconnectToDdpServer);
        connect(mDdp.get(), &DDPClient::methodRequested, this, &RocketChatAccount::parseMethodRequested);

        if (mSettings) {
            mDdp->setServerUrl(mSettings->serverUrl());
        }
        auto ddpclientAccountParameter = new DDPClient::DDPClientAccountParameter;
        ddpclientAccountParameter->settings = mSettings;
        ddpclientAccountParameter->logger = mRuqolaLogger;
        ddpclientAccountParameter->messageQueue = mMessageQueue;
        ddpclientAccountParameter->accountName = accountName();
        ddpclientAccountParameter->defaultAuthenticationInterface = defaultAuthenticationInterface();
        mDdp->setDDPClientAccountParameter(ddpclientAccountParameter);
        // Delay the call to mDdp->start() in case it emits disconnectedByServer right away
        // The caller doesn't expect ddp() to get reset to nullptr right away.
        QMetaObject::invokeMethod(mDdp.get(), &DDPClient::start, Qt::QueuedConnection);
    }
    return mDdp.get();
}

AuthenticationManager::LoginStatus RocketChatAccount::loginStatus() const
{
    if (Ruqola::useRestApiLogin()) {
        if (mRestApi) {
            if (mRestApi->authenticationManager()) {
                return mRestApi->authenticationManager()->loginStatus();
            }
        }
    } else {
        // TODO: DDP API should exist as soon as the hostname is known
        if (mDdp) {
            return mDdp->authenticationManager()->loginStatus();
        }
    }
    return AuthenticationManager::LoggedOut;
}

QString RocketChatAccount::loginStatusText() const
{
    return i18n("REST: %1\nDDP: %2", restLoginStatusText(), ddpLoginStatusText());
}

QString RocketChatAccount::restLoginStatusText() const
{
    if (mRestApi && mRestApi->authenticationManager()) {
        return AuthenticationManager::loginStatusToText(mRestApi->authenticationManager()->loginStatus());
    }
    return i18n("Not connected");
}

QString RocketChatAccount::ddpLoginStatusText() const
{
    if (mDdp) {
        if (!mDdp->isConnected())
            return i18n("Not connected yet");
        if (mDdp->authenticationManager()) {
            return AuthenticationManager::loginStatusToText(mDdp->authenticationManager()->loginStatus());
        }
    }
    return i18n("Not connected");
}

void RocketChatAccount::tryLogin()
{
    qCDebug(RUQOLA_RECONNECT_LOG) << accountName() << "attempting login" << mSettings->userName() << "on" << mSettings->serverUrl();
    Q_ASSERT(ddp());

    if (Ruqola::useRestApiLogin()) {
        if (auto interface = defaultAuthenticationInterface()) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "RESTAPI login" << accountName();
            if (!interface->login()) {
                qCDebug(RUQOLA_RECONNECT_LOG) << "RESTAPI impossible to login" << accountName();
                return;
            }

            // Normally, DDP is already connected at this point, and not yet logged in (it waits for the REST auth token)
            // But if it's in another state, recreate it.
            // Ex: we just changed networks, there's no NetworkManager support, the DDP socket
            // is stuck forever, and the user clicked logout to try and repair that... but it's stuck logging out.
            const auto ddpStatus = ddp()->authenticationManager()->loginStatus();
            if (ddpStatus == AuthenticationManager::LogoutOngoing || ddpStatus == AuthenticationManager::LogoutCleanUpOngoing) {
                qCDebug(RUQOLA_RECONNECT_LOG) << "DDP seems stuck, recreating it";
                mRoomModel->clear();
                mDdp.reset();
                ddp();
            }

        } else {
            qCWarning(RUQOLA_RECONNECT_LOG) << "No plugins loaded. Please verify your installation.";
        }
    } else {
        // ddp() creates a new DDPClient object if it doesn't exist.
        ddp()->enqueueLogin();
    }

    // In the meantime, load cache...
    mRoomModel->clear();
}

void RocketChatAccount::logOut()
{
    qCDebug(RUQOLA_RECONNECT_LOG) << "logout " << mSettings->userName() << "on" << mSettings->serverUrl();
    mSettings->logout();
    mRoomModel->clear();
    if (mRestApi) {
        if (!mRestApi->authenticationManager()->logoutAndCleanup(ownUser())) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "impossible to logout cleanup (restapi): " << accountName();
            mRestApi.reset();
        } // in the normal case, a job was launched and mRestApi will be deleted in slotRESTLoginStatusChanged
    }
    if (mDdp) {
        if (!mDdp->authenticationManager()->logoutAndCleanup(ownUser())) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "impossible to logout cleanup (ddp): " << accountName();
            mDdp.reset();
        }
    }
}

void RocketChatAccount::clearAllUnreadMessages()
{
    for (int roomIdx = 0, nRooms = mRoomModel->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = mRoomModel->index(roomIdx);
        const auto roomId = roomModelIndex.data(RoomModel::RoomId).toByteArray();
        const bool roomHasAlert = roomModelIndex.data(RoomModel::RoomAlert).toBool();
        if (roomHasAlert) {
            markRoomAsRead(roomId);
        }
    }
}

void RocketChatAccount::markRoomAsRead(const QByteArray &roomId)
{
    mMarkUnreadThreadsAsReadOnNextReply = true;
    restApi()->markRoomAsRead(roomId);
    if (threadsEnabled()) {
        getListMessages(roomId, ListMessagesModel::UnreadThreadsMessages);
    }
}

void RocketChatAccount::changeFavorite(const QByteArray &roomId, bool checked)
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

void RocketChatAccount::joinJitsiConfCall(const QByteArray &roomId)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::joinJitsiConfCall(const QString &roomId)" << roomId;
    // const QString hash = QString::fromLatin1(QCryptographicHash::hash((mRuqolaServerConfig->uniqueId() + roomId).toUtf8(), QCryptographicHash::Md5).toHex());
    const QString hash = mRuqolaServerConfig->uniqueId() + QString::fromLatin1(roomId);
#if defined(Q_OS_IOS)
    const QString scheme = QStringLiteral("org.jitsi.meet://");
#else
    const QString scheme = QStringLiteral("https://");
#endif
    const QString url = scheme + mRuqolaServerConfig->jitsiMeetUrl() + QLatin1Char('/') + mRuqolaServerConfig->jitsiMeetPrefix() + hash;
    const QUrl clickedUrl = QUrl::fromUserInput(url);
    RuqolaUtils::self()->openUrl(clickedUrl);
}

void RocketChatAccount::eraseRoom(const QByteArray &roomId, Room::RoomType channelType)
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

void RocketChatAccount::slotRoomOpenChanged(const QByteArray &rid)
{
    mManageChannels->verifyNeedSelectChannel(rid);
}

void RocketChatAccount::delaySelectChannelRequested(const QByteArray &rid)
{
    mManageChannels->delaySelectChannelRequested(rid);
}

void RocketChatAccount::initializeDirectChannel(const QByteArray &rid)
{
    ddp()->subscribeRoomMessage(rid);
    Q_EMIT selectRoomByRoomIdRequested(rid);
}

void RocketChatAccount::openDirectChannel(const QString &roomId)
{
    if (hasPermission(QStringLiteral("create-d"))) {
        auto job = new RocketChatRestApi::OpenDmJob(this);
        job->setDirectUserId(roomId);
        restApi()->initializeRestApiJob(job);
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

void RocketChatAccount::getRoomByTypeAndName(const QByteArray &rid, const QString &roomType)
{
    // it uses "/api/v1/method.call/getRoomByTypeAndName"
    // => use restapi for calling ddp method
    auto job = new RocketChatRestApi::MethodCallJob(this);
    RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
    info.methodName = QStringLiteral("getRoomByTypeAndName");
    info.anonymous = false;
    const QJsonArray params{{roomType}, {QString::fromLatin1(rid)}};
    info.messageObj = ddp()->generateJsonObject(info.methodName, params);
    job->setMethodCallJobInfo(std::move(info));
    restApi()->initializeRestApiJob(job);
    // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this](const QJsonObject &replyObject) {
        extractIdentifier(replyObject, "result"_L1, "_id"_L1);
        // qDebug() << " replyObject " << replyObject;
    });
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start getRoomByTypeAndName job";
    }
}

void RocketChatAccount::joinRoom(const QString &roomId, const QString &joinCode)
{
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    mManageChannels->channelJoin(info, joinCode);
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
    RocketChatRestApi::SetStatusJob::StatusType type = RocketChatRestApi::SetStatusJob::StatusType::Unknown;
    switch (status) {
    case User::PresenceStatus::Online:
        type = RocketChatRestApi::SetStatusJob::StatusType::OnLine;
        break;
    case User::PresenceStatus::Busy:
        type = RocketChatRestApi::SetStatusJob::StatusType::Busy;
        break;
    case User::PresenceStatus::Away:
        type = RocketChatRestApi::SetStatusJob::StatusType::Away;
        break;
    case User::PresenceStatus::Offline:
        type = RocketChatRestApi::SetStatusJob::StatusType::Offline;
        break;
    case User::PresenceStatus::Unknown:
        type = RocketChatRestApi::SetStatusJob::StatusType::Unknown;
        break;
    }
    mPresenceStatus = status;
    // qDebug() << "RocketChatAccount::setDefaultStatus  " << messageStatus;
    restApi()->setUserStatus(QString::fromLatin1(userId()), type, messageStatus);
}

QList<TextEmoticonsCore::CustomEmoji> RocketChatAccount::customEmojies() const
{
    QList<TextEmoticonsCore::CustomEmoji> mCustomEmojies;
    const auto customEmojiList = mEmojiManager->customEmojiList();
    for (const auto &emoji : customEmojiList) {
        TextEmoticonsCore::CustomEmoji custom;
        custom.setIdentifier(emoji.emojiIdentifier());
        custom.setIsAnimatedEmoji(emoji.isAnimatedImage());
        mCustomEmojies.append(std::move(custom));
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

void RocketChatAccount::userAutocomplete(const QString &searchText, const QString &exception)
{
    userCompleterModel()->clear();
    if (!searchText.isEmpty()) {
        const RocketChatRestApi::UsersAutocompleteJob::UsersAutocompleterInfo info{.pattern = searchText, .exception = exception};
        restApi()->usersAutocomplete(info);
    }
}

void RocketChatAccount::membersInRoom(const QByteArray &roomId, Room::RoomType channelType)
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
    const QByteArray roomId = roomData.value("_id"_L1).toString().toLatin1();
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    if (usersModelForRoom) {
        const int numberOfUsers = roomData.value("usersCount"_L1).toInt();
        if (usersModelForRoom->total() != numberOfUsers) {
            if (!usersModelForRoom->loadMoreUsersInProgress()) {
                usersModelForRoom->clear();
                usersModelForRoom->setLoadMoreUsersInProgress(true);
                const QString channelType = roomData.value("t"_L1).toString();
                restApi()->membersInRoom(roomId, channelType, 0, qMin(50, usersModelForRoom->offset()));
            }
        }
    }
}

void RocketChatAccount::parseUsersForRooms(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    // FIXME channelInfo
    const QString channelInfoIdentifier = channelInfo.identifier;
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(channelInfoIdentifier.toLatin1());
    if (usersModelForRoom) {
        usersModelForRoom->parseUsersForRooms(obj, mUserModel, true);
        usersModelForRoom->setLoadMoreUsersInProgress(false);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << channelInfoIdentifier;
    }
}

void RocketChatAccount::roomFiles(const QByteArray &roomId, Room::RoomType channelType)
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
    Room *room = mRoomModel->findRoom(channelInfo.identifier.toLatin1());
    if (room) {
        Roles r;
        r.parseRole(obj);
        room->setRolesForRooms(r);
        // qDebug() << " r " << r << " room " << room->name() << " obj" << obj;
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << channelInfo.identifier;
    }
}

void RocketChatAccount::slotGetThreadMessagesDone(const QJsonObject &obj, const QByteArray &threadMessageId)
{
    if (mThreadMessageModel->threadMessageId() != threadMessageId) {
        mThreadMessageModel->setThreadMessageId(threadMessageId);
        mThreadMessageModel->parseThreadMessages(obj);
    } else {
        mThreadMessageModel->loadMoreThreadMessages(obj);
    }
}

void RocketChatAccount::slotGetDiscussionsListDone(const QJsonObject &obj, const QByteArray &roomId)
{
    if (mDiscussionsModel->roomId() != roomId) {
        mDiscussionsModel->parseDiscussions(obj, roomId);
    } else {
        mDiscussionsModel->addMoreDiscussions(obj);
    }
    mDiscussionsModel->setLoadMoreDiscussionsInProgress(false);
}

void RocketChatAccount::slotGetListMessagesDone(const QJsonObject &obj, const QByteArray &roomId, ListMessagesModel::ListMessageType type)
{
    if (mMarkUnreadThreadsAsReadOnNextReply && type == ListMessagesModel::UnreadThreadsMessages) {
        qCDebug(RUQOLA_THREAD_MESSAGE_LOG) << "Obj" << obj << "roomId:" << roomId;
        mMarkUnreadThreadsAsReadOnNextReply = false;

        ListMessages messages;
        messages.parseMessages(obj, QStringLiteral("threads"));
        const auto listMessages = messages.listMessages();
        for (const auto &msg : listMessages) {
            QJsonObject params;
            params.insert(QStringLiteral("tmid"), QString::fromLatin1(msg.messageId()));
            ddp()->getThreadMessages(params);
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
    const QList<User> users = User::parseUsersList(obj, roleInfo());
    mUserCompleterModel->addUsers(users);
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

void RocketChatAccount::startConnecting()
{
    // Initiate DDP connection
    ddp();
    // Initiate first REST call, once we're out of Ruqola::self()
    QMetaObject::invokeMethod(mRocketChatBackend, &RocketChatBackend::loadServerInfo, Qt::QueuedConnection);
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

void RocketChatAccount::loadMoreUsersInRoom(const QByteArray &roomId, Room::RoomType channelType)
{
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    const int offset = usersModelForRoom->usersCount();
    if (offset < usersModelForRoom->total()) {
        usersModelForRoom->setLoadMoreUsersInProgress(true);
        restApi()->membersInRoom(roomId, Room::roomFromRoomType(channelType), offset, qMin(50, usersModelForRoom->total() - offset));
    }
}

void RocketChatAccount::getMentionsMessages(const QByteArray &roomId)
{
    mListMessageModel->clear();
    mListMessageModel->setRoomId(roomId);
    mListMessageModel->setLoadMoreListMessagesInProgress(true);
    restApi()->getMentionedMessages(roomId);
}

void RocketChatAccount::getPinnedMessages(const QByteArray &roomId)
{
    mListMessageModel->clear();
    mListMessageModel->setLoadMoreListMessagesInProgress(true);
    mListMessageModel->setRoomId(roomId);
    restApi()->getPinnedMessages(roomId);
}

void RocketChatAccount::getStarredMessages(const QByteArray &roomId)
{
    mListMessageModel->clear();
    mListMessageModel->setRoomId(roomId);
    mListMessageModel->setLoadMoreListMessagesInProgress(true);
    restApi()->getStarredMessages(roomId);
}

void RocketChatAccount::loadMoreFileAttachments(const QByteArray &roomId, Room::RoomType channelType)
{
    if (!mFilesModelForRoom->loadMoreFilesInProgress()) {
        const int offset = mFilesModelForRoom->fileAttachments()->filesCount();
        if (offset < mFilesModelForRoom->fileAttachments()->total()) {
            mFilesModelForRoom->setLoadMoreFilesInProgress(true);
            restApi()->filesInRoom(roomId, Room::roomFromRoomType(channelType), offset, qMin(50, mFilesModelForRoom->fileAttachments()->total() - offset));
        }
    }
}

void RocketChatAccount::loadMoreDiscussions(const QByteArray &roomId)
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

void RocketChatAccount::getListMessages(const QByteArray &roomId, ListMessagesModel::ListMessageType type)
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
#if 0
    for (int i = 0; i < array.count(); ++i) {
        const QJsonObject obj = array.at(i).toObject();
        const QString id = obj["_id"_L1].toString();
        const QString name = obj["name"_L1].toString();
        const QString username = obj["username"_L1].toString();
        // TODO
    }
#endif
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
            if (user.userId() != userId()) {
                mUserModel->addUser(user);
                mRoomModel->userStatusChanged(user);
            }
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

void RocketChatAccount::loadMoreListMessages(const QByteArray &roomId)
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
                // TODO allow to search by type
                restApi()->getThreadsList(roomId, false, offset, qMin(50, mListMessageModel->total() - offset), hasAtLeastVersion(7, 0, 0));
                break;
            case ListMessagesModel::UnreadThreadsMessages:
                restApi()->getThreadsList(roomId, true, offset, qMin(50, mListMessageModel->total() - offset), hasAtLeastVersion(7, 0, 0));
                break;
            }
        }
    }
}

void RocketChatAccount::loadThreadMessagesHistory(const QByteArray &threadMessageId)
{
    restApi()->getThreadMessages(threadMessageId);
}

void RocketChatAccount::createJitsiConfCall(const QByteArray &roomId)
{
    // TODO use restapi
    ddp()->createJitsiConfCall(roomId);
    joinJitsiConfCall(roomId);
}

void RocketChatAccount::addUserToRoom(const QByteArray &userId, const QByteArray &roomId, Room::RoomType channelType)
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

InputTextManager *RocketChatAccount::inputTextManager() const
{
    return mInputTextManager;
}

InputTextManager *RocketChatAccount::inputThreadMessageTextManager() const
{
    return mInputThreadMessageTextManager;
}

void RocketChatAccount::getThreadMessages(const QByteArray &threadMessageId, const Message &message)
{
    // mListMessageModel->clear();
    mThreadMessageModel->setPreviewMessage(message);
    restApi()->getThreadMessages(threadMessageId);
}

void RocketChatAccount::changeNotificationsSettings(const QByteArray &roomId,
                                                    RocketChatAccount::NotificationOptionsType notificationsType,
                                                    const QVariant &newValue)
{
    switch (notificationsType) {
    case NotificationOptionsType::DisableNotifications:
        restApi()->disableNotifications(roomId, newValue.toBool());
        break;
    case NotificationOptionsType::HideUnreadStatus:
        restApi()->hideUnreadStatus(roomId, newValue.toBool());
        break;
    case NotificationOptionsType::DesktopNotifications:
        restApi()->desktopNotifications(roomId, newValue.toString());
        break;
    case NotificationOptionsType::EmailNotifications:
        restApi()->emailNotifications(roomId, newValue.toString());
        break;
    case NotificationOptionsType::MobilePushNotifications:
        restApi()->mobilePushNotifications(roomId, newValue.toString());
        break;
    case NotificationOptionsType::UnreadAlert:
        restApi()->unreadAlert(roomId, newValue.toString());
        break;
    case NotificationOptionsType::MuteGroupMentions:
        restApi()->muteGroupMentions(roomId, newValue.toBool());
        break;
    case NotificationOptionsType::DesktopDurationNotifications:
        restApi()->desktopDurationNotifications(roomId, newValue.toInt());
        break;
    case NotificationOptionsType::DesktopSoundNotifications:
        restApi()->desktopSoundNotifications(roomId, newValue.toString());
        break;
    case NotificationOptionsType::HideMentionStatus:
        restApi()->hideMentionStatus(roomId, newValue.toBool());
        break;
    }
}

void RocketChatAccount::parseCustomSounds(const QJsonArray &obj)
{
    customSoundManager()->parseCustomSounds(obj);
}

void RocketChatAccount::parsePublicSettings(const QJsonObject &obj, bool update)
{
    mRuqolaServerConfig->parsePublicSettings(obj, update);
    parsePublicSettings();
    mPreviewUrlCacheManager->setEmbedCacheExpirationDays(mRuqolaServerConfig->embedCacheExpirationDays());
}

void RocketChatAccount::parsePublicSettings()
{
    if (!accountName().isEmpty()) {
        localDatabaseManager()->updateAccount(accountName(), mRuqolaServerConfig->serialize(false), LocalDatabaseUtils::currentTimeStamp());
    }

    fillAuthenticationModel();
    // Download logo/favicon if possible
    (void)faviconLogoUrlFromLocalCache(mRuqolaServerConfig->logoUrl().url);
    (void)faviconLogoUrlFromLocalCache(mRuqolaServerConfig->faviconUrl().url);

    Q_EMIT publicSettingChanged();
}

void RocketChatAccount::fillAuthenticationModel()
{
    QList<AuthenticationInfo> fillModel;
    // qDebug() << " before " << mLstInfos;
    for (int i = 0, total = mAuthenticationMethodInfos.count(); i < total; ++i) {
        if (mRuqolaServerConfig->canShowAuthMethod(mAuthenticationMethodInfos.at(i).oauthType())
            || (mAuthenticationMethodInfos.at(i).oauthType() == AuthenticationManager::AuthMethodType::PersonalAccessToken)) {
            fillModel.append(mAuthenticationMethodInfos.at(i));
        }
    }
    // qDebug() << "void RocketChatAccount::fillAuthenticationModel()  " << fillModel;
    mAccountAvailableAuthenticationMethodInfos = fillModel;
}

QList<AuthenticationInfo> RocketChatAccount::authenticationMethodInfos() const
{
    return mAccountAvailableAuthenticationMethodInfos;
}

void RocketChatAccount::setDefaultAuthentication(AuthenticationManager::AuthMethodType type)
{
    PluginAuthenticationInterface *interface = mLstPluginAuthenticationInterface.value(type);
    if (interface) {
        mDefaultAuthenticationInterface = interface;
    } else {
        qCWarning(RUQOLA_LOG) << "No interface defined for  " << type;
    }
}

void RocketChatAccount::initializeAuthenticationPlugins()
{
    // TODO change it when we change server
    // Clean up at the end.
    const QList<PluginAuthentication *> lstPlugins = AuthenticationManager::self()->pluginsList();
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::initializeAuthenticationPlugins()" << lstPlugins.count();
    if (lstPlugins.isEmpty()) {
        qCWarning(RUQOLA_LOG) << " No plugins loaded. Please verify your installation.";
        if (Ruqola::useRestApiLogin()) {
            restApi()->authenticationManager()->setLoginStatus(AuthenticationManager::FailedToLoginPluginProblem);
        } else {
            ddp()->authenticationManager()->setLoginStatus(AuthenticationManager::FailedToLoginPluginProblem);
        }
        return;
    }
    mLstPluginAuthenticationInterface.clear();

    mAuthenticationMethodInfos.clear();
    for (PluginAuthentication *abstractPlugin : lstPlugins) {
        AuthenticationInfo info;
        info.setIconName(abstractPlugin->iconName());
        info.setName(abstractPlugin->name());
        info.setOauthType(abstractPlugin->authenticationType());
        if (info.isValid()) {
            mAuthenticationMethodInfos.append(std::move(info));
        }

        PluginAuthenticationInterface *interface = abstractPlugin->createInterface(this);
        interface->setAccount(this);
        mRuqolaServerConfig->addRuqolaAuthenticationSupport(abstractPlugin->authenticationType());
        mLstPluginAuthenticationInterface.insert(abstractPlugin->authenticationType(), interface);
        // For the moment initialize default interface
        if (abstractPlugin->authenticationType() == AuthenticationManager::AuthMethodType::Password) {
            mDefaultAuthenticationInterface = interface;
        }
        qCDebug(RUQOLA_LOG) << " plugin type " << abstractPlugin->authenticationType();
    }
    // TODO fill ??? or store QList<AuthenticationInfo>
}

PluginAuthenticationInterface *RocketChatAccount::defaultAuthenticationInterface() const
{
    return mDefaultAuthenticationInterface;
}

QString RocketChatAccount::userName() const
{
    return mSettings->userName();
}

void RocketChatAccount::setAccountName(const QString &accountname)
{
    // Initialize new account room
    // qDebug() << "void RocketChatAccount::setAccountName(const QString &accountname)"<<accountname;
    loadSettings(ManagerDataPaths::self()->accountConfigFileName(accountname));
    mSettings->setAccountName(accountname);
}

QString RocketChatAccount::accountName() const
{
    return mSettings->accountName();
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
    mPreviewUrlCacheManager->setEmbedCacheExpirationDays(mRuqolaServerConfig->embedCacheExpirationDays());
}

void RocketChatAccount::permissionUpdated(const QJsonArray &replyArray)
{
    if (mPermissionManager.updatePermission(replyArray)) {
        Q_EMIT permissionChanged();
    }
    // TODO stockage roles! Load it before permission!
    // QJsonObject({"args":["updated",{"_id":"access-mailer","_updatedAt":{"$date":1634569746270},"roles":["admin","vFXCWG9trXLti6xQm"]}],"eventName":"permissions-changed"})
}

const QList<RoleInfo> &RocketChatAccount::roleInfo() const
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
        const QString infoStr = obj["string"_L1].toString();
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

QByteArray RocketChatAccount::userId() const
{
    return mSettings->userId();
}

bool RocketChatAccount::accountEnabled() const
{
    return mSettings->accountEnabled();
}

QString RocketChatAccount::serverUrl() const
{
    return mSettings->serverUrl();
}

void RocketChatAccount::setServerUrl(const QString &serverURL)
{
    mSettings->setServerUrl(serverURL);
    restApi()->setServerUrl(serverURL);
    mEmojiManager->setServerUrl(serverURL);
}

QUrl RocketChatAccount::urlForLink(const QString &link) const
{
    if (link.startsWith("https:"_L1) || link.startsWith("http:"_L1)) {
        return QUrl(link);
    }
    QString tmpUrl = mSettings->serverUrl();
    if (!tmpUrl.startsWith("https://"_L1)) {
        tmpUrl = "https://"_L1 + tmpUrl;
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

QUrl RocketChatAccount::avatarUrlFromLocalCache(const QString &url)
{
    return mCache->avatarUrlFromLocalCache(url);
}

QUrl RocketChatAccount::soundUrlFromLocalCache(const QString &url)
{
    return mCache->soundUrlFromLocalCache(url);
}

QUrl RocketChatAccount::faviconLogoUrlFromLocalCache(const QString &url)
{
    return mCache->faviconLogoUrlFromLocalCache(url);
}

QUrl RocketChatAccount::previewUrlFromLocalCache(const QString &url)
{
    return mCache->previewUrlFromLocalCache(url);
}

QUrl RocketChatAccount::attachmentUrlFromLocalCache(const QString &url)
{
    return mCache->attachmentUrlFromLocalCache(url);
}

bool RocketChatAccount::attachmentIsInLocalCache(const QString &url)
{
    return mCache->attachmentIsInLocalCache(url);
}

void RocketChatAccount::loadHistory(const QByteArray &roomID, bool initial, qint64 timeStamp)
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
        info.roomId = QString::fromLatin1(roomID);
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

bool RocketChatAccount::otrEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::OtrEnabled;
}

bool RocketChatAccount::allowProfileChange() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowUserProfileChange;
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

bool RocketChatAccount::registrationFormEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::RegistrationFormEnabled;
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

bool RocketChatAccount::federationEnabled() const
{
    return mRuqolaServerConfig->federationEnabled();
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

void RocketChatAccount::setRoomListDisplay(OwnUserPreferences::RoomListDisplay roomListDisplay)
{
    RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    info.userId = userId();
    switch (roomListDisplay) {
    case OwnUserPreferences::RoomListDisplay::Medium:
        info.sidebarViewMode = QStringLiteral("medium");
        break;
    case OwnUserPreferences::RoomListDisplay::Condensed:
        info.sidebarViewMode = QStringLiteral("condensed");
        break;
    case OwnUserPreferences::RoomListDisplay::Extended:
        info.sidebarViewMode = QStringLiteral("extended");
        break;
    case OwnUserPreferences::RoomListDisplay::Unknown:
        qCWarning(RUQOLA_LOG) << " OwnUserPreferences::setRoomListDisplay::Unknown is a bug";
        return;
    }
    setUserPreferences(std::move(info));
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

OwnUserPreferences::RoomListDisplay RocketChatAccount::roomListDisplay() const
{
    return ownUser().ownUserPreferences().roomListDisplay();
}

void RocketChatAccount::kickUser(const QByteArray &roomId, const QByteArray &userId, Room::RoomType channelType)
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

void RocketChatAccount::rolesInRoom(const QByteArray &roomId, Room::RoomType channelType)
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

void RocketChatAccount::switchingToRoom(const QByteArray &roomID)
{
    clearTypingNotification();
    checkInitializedRoom(roomID);
}

void RocketChatAccount::changeRoles(const QByteArray &roomId, const QString &userId, Room::RoomType channelType, RocketChatAccount::RoleType roleType)
{
    switch (channelType) {
    case Room::RoomType::Private:
        switch (roleType) {
        case RocketChatAccount::RoleType::AddOwner:
            restApi()->groupAddOwner(roomId, userId);
            break;
        case RocketChatAccount::RoleType::AddLeader:
            restApi()->groupAddLeader(roomId, userId);
            break;
        case RocketChatAccount::RoleType::AddModerator:
            restApi()->groupAddModerator(roomId, userId);
            break;
        case RocketChatAccount::RoleType::RemoveOwner:
            restApi()->groupRemoveOwner(roomId, userId);
            break;
        case RocketChatAccount::RoleType::RemoveLeader:
            restApi()->groupRemoveLeader(roomId, userId);
            break;
        case RocketChatAccount::RoleType::RemoveModerator:
            restApi()->groupRemoveModerator(roomId, userId);
            break;
        }

        break;
    case Room::RoomType::Channel:
        switch (roleType) {
        case RocketChatAccount::RoleType::AddOwner:
            restApi()->channelAddOwner(roomId, userId);
            break;
        case RocketChatAccount::RoleType::AddLeader:
            restApi()->channelAddLeader(roomId, userId);
            break;
        case RocketChatAccount::RoleType::AddModerator:
            restApi()->channelAddModerator(roomId, userId);
            break;
        case RocketChatAccount::RoleType::RemoveOwner:
            restApi()->channelRemoveOwner(roomId, userId);
            break;
        case RocketChatAccount::RoleType::RemoveLeader:
            restApi()->channelRemoveLeader(roomId, userId);
            break;
        case RocketChatAccount::RoleType::RemoveModerator:
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
    const bool canEditMessage = hasPermission(QStringLiteral("edit-message"), message.roomId());
    const bool isEditAllowed = allowEditingMessages();
    const bool editOwn = message.userId() == userId();

    if (!(canEditMessage || (isEditAllowed && editOwn))) {
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
    qCDebug(RUQOLA_LOG) << " RocketChatAccount::parseVideoConference(const QJsonArray &contents) " << contents;
    mVideoConferenceManager->parseVideoConference(contents);
}

void RocketChatAccount::parseOtr(const QJsonArray &contents)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::parseOtr(const QJsonArray &contents)" << contents << " account name" << accountName();
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
    info.setDateTime(QDateTime::currentDateTime());
    info.parseNotification(contents);
    if (!info.isValid()) {
        qCWarning(RUQOLA_LOG) << " Info is invalid ! " << contents;
    } else {
        switch (info.notificationType()) {
        case NotificationInfo::NotificationType::StandardMessage: {
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
        case NotificationInfo::NotificationType::ConferenceCall: {
            // Nothing
            break;
        }
        }

        Q_EMIT notification(info);
    }
}

void RocketChatAccount::inputAutocomplete(const QByteArray &roomId,
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

void RocketChatAccount::ignoreUser(const QByteArray &rid, const QByteArray &userId, bool ignore)
{
    restApi()->ignoreUser(rid, userId, ignore);
}

void RocketChatAccount::muteUser(const QByteArray &rid, const QString &userName, bool mute)
{
    restApi()->muteUser(rid, userName, mute);
}

void RocketChatAccount::blockUser(const QString &rid, bool block)
{
    // TODO use restapi
    if (rid.isEmpty()) {
        qCWarning(RUQOLA_LOG) << " void RocketChatAccount::blockUser EMPTY rid ! block " << block;
    } else {
        // qDebug() << " void RocketChatAccount::blockUser userId " << userId << " block " << block << " rid " << rid << " own userdId" << userID();

        const QString userIdFromDirectChannel = Utils::userIdFromDirectChannel(rid, QString::fromLatin1(userId()));
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

void RocketChatAccount::checkInitializedRoom(const QByteArray &roomId)
{
    Room *r = mRoomModel->findRoom(roomId);
    if (r && !r->wasInitialized()) {
        r->setWasInitialized(true);
        if (ddp()) {
            ddp()->subscribeRoomMessage(roomId);
            if (!r->archived()) {
                membersInRoom(r->roomId(), r->channelType());
                rolesInRoom(r->roomId(), r->channelType());
                if (r->channelType() == Room::RoomType::Channel) {
                    restApi()->getChannelsCounter(r->roomId());
                }
            }
            loadHistory(r->roomId(), true /*initial loading*/);
        }
    } else if (!r) {
        qWarning() << " Room " << roomId << " was no found! Need to open it";
        // openDirectChannel(roomId);
    }
}

void RocketChatAccount::openDocumentation()
{
    RuqolaUtils::self()->openUrl(QUrl(QStringLiteral("help:/")));
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
        if (obj.contains("username"_L1)) {
            const QString userName = obj["username"_L1].toString();
            Utils::AvatarInfo info;
            info.avatarType = Utils::AvatarType::User;
            info.identifier = userName;
            Q_EMIT avatarWasChanged(info);
        } else if (obj.contains("rid"_L1)) {
            const QString roomId = obj["rid"_L1].toString();
            const QString etag = obj["etag"_L1].toString();
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
        const QByteArray scope = obj["scope"_L1].toString().toLatin1();
        Room *room = mRoomModel->findRoom(scope);
        if (room) {
            room->updateRoles(obj);
        } else {
            qWarning() << " Impossible to find room associate to " << scope << contents;
        }
    }
}

void RocketChatAccount::threadsInRoom(const QByteArray &roomId, bool onlyUnread)
{
    if (threadsEnabled()) {
        mListMessageModel->clear();
        mListMessageModel->setRoomId(roomId);
        restApi()->getThreadsList(roomId, onlyUnread, 0, 50, hasAtLeastVersion(7, 0, 0));
    }
}

void RocketChatAccount::discussionsInRoom(const QByteArray &roomId)
{
    mDiscussionsModel->initialize();
    mDiscussionsModel->setLoadMoreDiscussionsInProgress(true);
    restApi()->getDiscussions(roomId);
}

void RocketChatAccount::getSupportedLanguages()
{
    if (autoTranslateEnabled()) {
        const bool needTargetLanguage = true;
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

void RocketChatAccount::slotUsersPresenceDone(const QJsonObject &obj)
{
    // qDebug() << " void RocketChatAccount::slotUsersPresenceDone(const QJsonObject &obj)" << obj;
    const auto lst = obj.value("users"_L1).toArray();
    for (const auto &var : lst) {
        const QJsonObject userJson = var.toObject();
        User user;
        user.parseUserRestApi(userJson, roleInfo());
        if (user.isValid()) {
            userStatusChanged(user);
        }
    }
}

void RocketChatAccount::slotReconnectToDdpServer() // connected to DDPClient::disconnectedByServer
{
    mRoomModel->clear();
    if (Ruqola::useRestApiLogin()) {
        if (mRestApi && mRestApi->authenticationManager()->isLoggedIn()) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "Reconnect only ddpclient";
            ddp()->enqueueLogin();
        }
    } else {
        // ddp() creates a new DDPClient object if it doesn't exist.
        ddp()->enqueueLogin();
    }
}

E2eKeyManager *RocketChatAccount::e2eKeyManager() const
{
    return mE2eKeyManager;
}

AppsCategoriesModel *RocketChatAccount::appsCategoriesModel() const
{
    return mAppsCategoriesModel;
}

AppsMarketPlaceModel *RocketChatAccount::appsMarketPlaceModel() const
{
    return mAppsMarketPlaceModel;
}

void RocketChatAccount::autoReconnectDelayed()
{
    qCDebug(RUQOLA_RECONNECT_LOG) << "starting single shot timer with" << mDelayReconnect << "ms" << "account name" << accountName();
    // Clear auth token otherwise we can't reconnect.
    mSettings->setAuthToken({});

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
        reconnectToServer();
    });
}

void RocketChatAccount::initializeAccount()
{
    listEmojiCustom();

    // load when necessary
    restApi()->usersPresence();
    // Force set online.
    // TODO don't reset message status !
    if (RuqolaGlobalConfig::self()->setOnlineAccounts()) {
        ddp()->setDefaultStatus(User::PresenceStatus::Online);
    }
    // Initialize sounds
    mCustomSoundManager->initializeDefaultSounds();
    ddp()->listCustomSounds();
    restApi()->customUserStatus();
    slotLoadRoles();
    checkLicenses();
    qDebug() << "initializeAccount: encryptionEnabled =" << encryptionEnabled() << "account name" << accountName();
    if (encryptionEnabled()) {
        mE2eKeyManager->fetchMyKeys();
    }

    Q_EMIT accountInitialized();
}

bool RocketChatAccount::hasAtLeastVersion(int major, int minor, int patch) const
{
    return ruqolaServerConfig()->hasAtLeastVersion(major, minor, patch);
}

void RocketChatAccount::checkLicenses()
{
    if (ruqolaServerConfig()->hasAtLeastVersion(6, 5, 0)) {
        auto job = new RocketChatRestApi::LicensesInfoJob(this);
        restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::LicensesInfoJob::licensesInfoDone, this, [this](const QJsonObject &obj) {
            const QJsonObject license = obj["license"_L1].toObject();
            if (!license.isEmpty()) {
                const bool isEnterprise = !license["activeModules"_L1].toArray().isEmpty();
                mRuqolaServerConfig->setHasEnterpriseSupport(isEnterprise);
                if (isEnterprise) {
                    licenseGetModules();
                }
            }
        });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start LicensesInfoJob job";
        }
    } else {
        auto job = new RocketChatRestApi::LicensesIsEnterpriseJob(this);
        restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::LicensesIsEnterpriseJob::licensesIsEnterpriseDone, this, [this](bool isEnterprise) {
            mRuqolaServerConfig->setHasEnterpriseSupport(isEnterprise);
            if (isEnterprise) {
                licenseGetModules();
            }
        });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start LicensesIsEnterpriseJob job";
        }
    }
}

void RocketChatAccount::licenseGetModules()
{
    if (mRuqolaServerConfig->hasAtLeastVersion(6, 5, 0)) {
        auto job = new RocketChatRestApi::LicensesInfoJob(this);
        restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::LicensesInfoJob::licensesInfoDone, this, [this](const QJsonObject &obj) {
            const QJsonObject license = obj["license"_L1].toObject();
            const QJsonArray activeModules = license["activeModules"_L1].toArray();
            parseLicenses(activeModules);
        });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start LicensesInfoJob job";
        }
    } else {
        ddp()->licenseGetModules();
    }
}

NotificationPreferences *RocketChatAccount::notificationPreferences() const
{
    return mNotificationPreferences;
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
    auto job = new RocketChatRestApi::ListCommandsJob(this);
    restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::ListCommandsJob::listCommandsDone, this, &RocketChatAccount::slotListCommandDone);
    if (!job->start()) {
        qCDebug(RUQOLA_LOG) << "Impossible to start ListPermissionsJob job";
    }
}

QString RocketChatAccount::getTranslatedIdentifier(const QString &lang, const QString &identifier) const
{
    return mDownloadAppsLanguagesManager->translatedString(lang, identifier);
}

void RocketChatAccount::slotListCommandDone(const QJsonObject &obj)
{
    Commands commands;
    commands.setDownloadManager(mDownloadAppsLanguagesManager);
    commands.parseCommands(obj);
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

bool RocketChatAccount::runCommand(const QString &msg, const QByteArray &roomId, const QByteArray &tmid)
{
    const RocketChatRestApi::RunCommandJob::RunCommandInfo info = RocketChatRestApi::RunCommandJob::parseString(msg, roomId, tmid);
    if (info.isValid()) {
        restApi()->runCommand(info);
        return true;
    }
    return false;
}

User RocketChatAccount::fullUserInfo(const QString &userName) const
{
    return mUserModel->fullUserInfo(userName);
}

void RocketChatAccount::slotDDpLoginStatusChanged()
{
    if (mDdp && mDdp->authenticationManager()->loginStatus() == AuthenticationManager::LoggedOutAndCleanedUp) {
        qCDebug(RUQOLA_RECONNECT_LOG) << "Logged out from DDP, resetting mDdp" << accountName();
        disconnect(mDdp.get(), nullptr, this, nullptr);
        mDdp.release()->deleteLater();
    }
    if (!Ruqola::useRestApiLogin()) {
        slotLoginStatusChanged();
    }
    Q_EMIT ddpLoginStatusChanged();
    if (!mRestApi && !mDdp) {
        logoutCompleted();
    }
}

void RocketChatAccount::slotRESTLoginStatusChanged()
{
    const auto loginStatus = mRestApi->authenticationManager()->loginStatus();
    if (loginStatus == AuthenticationManager::LoggedOutAndCleanedUp) {
        qCDebug(RUQOLA_RECONNECT_LOG) << "Logged out from REST, resetting mRestApi" << accountName();
        disconnect(mRestApi.get(), nullptr, this, nullptr);
        mRestApi.release()->deleteLater();
    } else if (loginStatus == AuthenticationManager::LoggedIn) {
        // Reset it.
        mDelayReconnect = 100;
    } else if (loginStatus == AuthenticationManager::GenericError) {
        // Clear authToken it can be changed
        // If we don't clear it ruqola will want to login with resume method => it will failed all the time.
        mSettings->setAuthToken({});
    }
    Q_EMIT loginStatusChanged();
    if (!mRestApi && !mDdp) {
        logoutCompleted();
    }
}

void RocketChatAccount::logoutCompleted()
{
    qCDebug(RUQOLA_RECONNECT_LOG) << "Successfully logged out from" << accountName();
    Q_EMIT logoutDone(accountName());
}

bool RocketChatAccount::e2EPasswordMustBeDecrypt() const
{
    return mE2EPasswordMustBeDecrypt;
}

void RocketChatAccount::setE2EPasswordMustBeDecrypt(bool newE2EPasswordMustBeDecrypt)
{
    mE2EPasswordMustBeDecrypt = newE2EPasswordMustBeDecrypt;
}

void RocketChatAccount::setLastSelectedRoom(const QByteArray &roomId)
{
    Room *r = room(roomId);
    if (r) {
        r->setLastOpenedAt(QDateTime::currentSecsSinceEpoch());
    }
    mSettings->setLastSelectedRoom(roomId);
}

bool RocketChatAccount::e2EPasswordMustBeSave() const
{
    return mE2EPasswordMustBeSave;
}

void RocketChatAccount::setE2EPasswordMustBeSave(bool newE2EPasswordMustBeSave)
{
    mE2EPasswordMustBeSave = newE2EPasswordMustBeSave;
}

void RocketChatAccount::slotLoginStatusChanged() // only used in DDP-only mode (!useRestApiLogin)
{
    if (loginStatus() == AuthenticationManager::LoggedIn) {
        // Reset it.
        mDelayReconnect = 100;
        qCDebug(RUQOLA_RECONNECT_LOG) << "Successfully logged in!";
    } else if (loginStatus() == AuthenticationManager::LoginFailedInvalidUserOrPassword) {
        // clear auth token to refresh it with the next login
        mSettings->setAuthToken({});
    }
    Q_EMIT loginStatusChanged();
}

void RocketChatAccount::registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo)
{
    restApi()->registerNewUser(userInfo);
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
    Q_EMIT ownUserUiPreferencesChanged();
}

bool RocketChatAccount::isAdministrator() const
{
    return mOwnUser.isAdministrator();
}

void RocketChatAccount::slotChannelGetCountersDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    Room *room = mRoomModel->findRoom(channelInfo.identifier.toLatin1());
    if (room) {
        ChannelCounterInfo info;
        info.parseCounterInfo(obj);
        room->setChannelCounterInfo(info);
    }
}

void RocketChatAccount::slotMarkAsReadDone(const QByteArray &roomId)
{
    Room *room = this->room(roomId);
    if (room) {
        room->setChannelCounterInfo({});
    }
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

void RocketChatAccount::extractIdentifier(const QJsonObject &replyObject, const QString &identifier, const QString &subIdentifier)
{
    const QJsonObject channel = replyObject[identifier].toObject();
    const QString roomId = channel[subIdentifier].toString();
    if (!roomId.isEmpty()) {
        Q_EMIT selectRoomByRoomIdRequested(roomId.toLatin1());
    } else {
        qCWarning(RUQOLA_LOG) << "No identifier found " << identifier << " with subIdentifier " << subIdentifier << " replyObject " << replyObject;
    }
}

void RocketChatAccount::slotCreateGroupDone(const QJsonObject &replyObject)
{
    extractIdentifier(replyObject, "group"_L1, "_id"_L1);
}

void RocketChatAccount::slotCreateChannelDone(const QJsonObject &replyObject)
{
    extractIdentifier(replyObject, "channel"_L1, "_id"_L1);
}

void RocketChatAccount::slotPostMessageDone(const QJsonObject &replyObject)
{
    // qDebug() << "replyObject " << replyObject;
    const QJsonObject roomInfo = replyObject["message"_L1].toObject();
    addMessage(roomInfo, true, true);
}

void RocketChatAccount::updateUserData(const QJsonArray &contents)
{
    qDebug() << " void RocketChatAccount::updateUserData(const QJsonArray &contents)" << contents;
    for (const auto &array : contents) {
        const QJsonObject updateJson = array["diff"_L1].toObject();
        const QStringList keys = updateJson.keys();
        OwnUserPreferences ownUserPreferences = mOwnUser.ownUserPreferences();
        for (const QString &key : keys) {
            if (key == "settings.preferences.highlights"_L1) {
                const QJsonArray highlightsArray = updateJson.value(key).toArray();
                ownUserPreferences.updateHighlightWords(highlightsArray);
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == "settings.preferences.enableAutoAway"_L1) {
                ownUserPreferences.setEnableAutoAway(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
            } else if (key == "settings.preferences.convertAsciiEmoji"_L1) {
                ownUserPreferences.setConvertAsciiEmoji(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == "settings.preferences.hideRoles"_L1) {
                ownUserPreferences.setHideRoles(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == "settings.preferences.displayAvatars"_L1) {
                ownUserPreferences.setDisplayAvatars(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT needUpdateMessageView();
            } else if (key == "settings.preferences.sidebarViewMode"_L1) { // Channel List view mode
                const QString value = updateJson.value(key).toString();
                if (value == "medium"_L1) {
                    ownUserPreferences.setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Medium);
                } else if (value == "condensed"_L1) {
                    ownUserPreferences.setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Condensed);
                } else if (value == "extended"_L1) {
                    ownUserPreferences.setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Extended);
                } else {
                    qCWarning(RUQOLA_LOG) << "RoomListDisplay is not defined ?  " << value;
                }
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserUiPreferencesChanged();
            } else if (key == "settings.preferences.sidebarShowUnread"_L1) {
                ownUserPreferences.setShowUnread(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserUiPreferencesChanged();
            } else if (key == "settings.preferences.sidebarDisplayAvatar"_L1) { // Avatar in channel list view
                ownUserPreferences.setShowRoomAvatar(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserUiPreferencesChanged();
            } else if (key == "settings.preferences.sidebarShowFavorites"_L1) {
                ownUserPreferences.setShowFavorite(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserUiPreferencesChanged();
            } else if (key == "settings.preferences.sidebarSortby"_L1) {
                const QString value = updateJson.value(key).toString();
                if (value == "activity"_L1) {
                    ownUserPreferences.setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::ByLastMessage);
                } else if (value == "alphabetical"_L1) {
                    ownUserPreferences.setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::Alphabetically);
                } else {
                    qCWarning(RUQOLA_LOG) << "Sortby is not defined ?  " << value;
                }
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserUiPreferencesChanged();
            } else if (key == "settings.preferences.desktopNotifications"_L1) {
                ownUserPreferences.setDesktopNotifications(updateJson.value(key).toString());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == "settings.preferences.pushNotifications"_L1) {
                ownUserPreferences.setPushNotifications(updateJson.value(key).toString());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == "settings.preferences.emailNotificationMode"_L1) {
                ownUserPreferences.setEmailNotificationMode(updateJson.value(key).toString());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == "settings.preferences.newMessageNotification"_L1) {
                ownUserPreferences.setNewMessageNotification(updateJson.value(key).toString().toLatin1());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == "settings.preferences.newRoomNotification"_L1) {
                ownUserPreferences.setNewRoomNotification(updateJson.value(key).toString().toLatin1());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == "settings.preferences.notificationsSoundVolume"_L1) {
                ownUserPreferences.setNotificationsSoundVolume(updateJson.value(key).toInt());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
                // Update notification volume!
                mSoundManager->setVolume(mOwnUser.ownUserPreferences().notificationsSoundVolume());
            } else if (key == "settings.preferences.muteFocusedConversations"_L1) {
                ownUserPreferences.setMuteFocusedConversations(updateJson.value(key).toBool());
                mOwnUser.setOwnUserPreferences(ownUserPreferences);
                Q_EMIT ownUserPreferencesChanged();
            } else if (key == "e2e.private_key"_L1) {
                // TODO update private key!!!!!
                qDebug() << " e2e.private_key changed !!! " << updateJson.value(key).toString();
            } else {
                const static QRegularExpression bannerRegularExpression(QStringLiteral("banners.(.*).read"));
                QRegularExpressionMatch rmatch;
                if (key.contains(bannerRegularExpression, &rmatch)) {
                    if (rmatch.hasMatch()) {
                        const QString bannerName = rmatch.captured(1);
                        const bool result = updateJson.value(key).toBool();
                        mBannerInfos.updateBannerReadInfo(bannerName.toLatin1(), result);
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
    const QByteArray roomId = replyObject.value("rid"_L1).toString().toLatin1();
    if (!roomId.isEmpty()) {
        MessagesModel *messageModel = messageModelForRoom(roomId);
        if (!messageModel) {
            qCWarning(RUQOLA_LOG) << "Unexpected null message model.";
            return;
        }
        Message m;
        m.parseMessage(replyObject, useRestApi, emojiManager());
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

void RocketChatAccount::setAvatarUrl(const QString &url)
{
    RocketChatRestApi::UserBaseJob::UserInfo userInfo;
    userInfo.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
    userInfo.userIdentifier = QString::fromLatin1(userId());
    RocketChatRestApi::SetAvatarJob::SetAvatarInfo avatarInfo;
    avatarInfo.mAvatarUrl = url;
    restApi()->setAvatar(userInfo, avatarInfo);
}

void RocketChatAccount::setImageUrl(const QUrl &url)
{
    RocketChatRestApi::UserBaseJob::UserInfo userInfo;
    userInfo.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
    userInfo.userIdentifier = QString::fromLatin1(userId());
    RocketChatRestApi::SetAvatarJob::SetAvatarInfo avatarInfo;
    avatarInfo.mImageUrl = url;
    restApi()->setAvatar(userInfo, avatarInfo);
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

bool RocketChatAccount::hasPermission(const QString &permissionId, const QByteArray &roomId) const
{
    QStringList currentRoles;
    if (roomId.isEmpty()) {
        currentRoles = mOwnUser.roles();
    } else {
        Room *r = room(roomId);
        if (r) {
            currentRoles = r->roles();
        }
    }
    const QStringList permissionRoles{mPermissionManager.roles(permissionId)};
    if (permissionRoles.isEmpty()) { // Check if we don't have stored permissionId in permission manager
        if (!mPermissionManager.contains(permissionId)) {
            qCWarning(RUQOLA_LOG) << "permissionId not loaded during setup:" << permissionId;
        }
    }
    for (const QString &role : permissionRoles) {
        if (currentRoles.contains(role)) {
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
    const QJsonObject user = replyObject.value("user"_L1).toObject();
    if (user.value("_id"_L1).toString().toLatin1() == userId()) {
        OwnUserPreferences ownUserPreferences;
        ownUserPreferences.parsePreferences(user.value("settings"_L1).toObject().value("preferences"_L1).toObject());
        mOwnUser.setOwnUserPreferences(ownUserPreferences);
        Q_EMIT ownUserUiPreferencesChanged();
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
    restApi()->setUserStatus(QString::fromLatin1(userId()),
                             away ? RocketChatRestApi::SetStatusJob::StatusType::Away : RocketChatRestApi::SetStatusJob::StatusType::OnLine,
                             {});
    qCDebug(RUQOLA_LOG) << "RocketChatAccount::slotAwayStatusChanged  " << away;
}

void RocketChatAccount::generate2FaTotp(const QJsonObject &obj)
{
    // qDebug() << "RocketChatAccount::generate2FaTotp " << obj;
    const QString secret = obj.value("secret"_L1).toString();
    const QString url = obj.value("url"_L1).toString();
    Q_EMIT totpResult(secret, url);
}

void RocketChatAccount::totpDisabledVerify(const QJsonObject &root)
{
    const int result = root.value("result"_L1).toInt();
    Q_EMIT disabledTotpValid(result == 1);
}

void RocketChatAccount::totpVerify(const QJsonObject &obj)
{
    if (obj.isEmpty()) {
        Q_EMIT totpInvalid();
    } else {
        // qDebug() << "totpValid " << obj;
        QStringList lstCodes;
        const QJsonArray codes = obj.value("codes"_L1).toArray();
        const auto nbCodes{codes.count()};
        lstCodes.reserve(nbCodes);
        for (auto i = 0; i < nbCodes; ++i) {
            lstCodes.append(codes.at(i).toString());
        }
        Q_EMIT totpValid(lstCodes);
    }
}

bool RocketChatAccount::hasLicense(const QString &name)
{
    return mLicensesManager.hasLicense(name);
}

void RocketChatAccount::parseLicenses(const QJsonArray &replyArray)
{
    // qDebug() << " replyArray " << replyArray;
    mLicensesManager.parseLicenses(replyArray);
}

void RocketChatAccount::addMessageToDataBase(const QString &roomName, const Message &message)
{
    mLocalDatabaseManager->addMessage(accountName(), roomName, message);
}

void RocketChatAccount::deleteMessageFromDatabase(const QString &roomName, const QByteArray &messageId)
{
    mLocalDatabaseManager->deleteMessage(accountName(), roomName, messageId);
}

// Only for debugging permissions. (debug mode)
QList<Permission> RocketChatAccount::permissions() const
{
    return mPermissionManager.permissions();
}

void RocketChatAccount::playSound(const QByteArray &soundIdentifier)
{
    if (!soundIdentifier.isEmpty()) {
        const QString url = customSoundManager()->soundFilePath(soundIdentifier);
        if (!url.isEmpty()) {
            mSoundManager->playSound(soundUrlFromLocalCache(url));
        }
    }
}

void RocketChatAccount::playNewRoomNotification()
{
    const QByteArray identifier = mOwnUser.ownUserPreferences().newRoomNotification();
    playSound(identifier);
}

bool RocketChatAccount::allowCustomStatusMessage() const
{
    return mRuqolaServerConfig->allowCustomStatusMessage();
}

bool RocketChatAccount::appMarketPlaceLoaded() const
{
    return mAppsMarketPlaceModel->wasFilled();
}

void RocketChatAccount::loadAppCount()
{
    if (isAdministrator()) {
        if (mAppsCategoriesModel->wasFilled()) {
            return;
        }
        auto job = new RocketChatRestApi::AppCountJob(this);
        restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::AppCountJob::appCountDone, this, [this](const QJsonObject &obj) {
            AppsCountInfo countinfo;
            countinfo.parseCountInfo(obj);
            mAppsMarketPlaceModel->setAppsCountInfo(countinfo);
            loadAppCategories();
            Q_EMIT appsCountLoadDone();
        });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start AppCountJob";
        }
    } else {
        loadAppCategories();
    }
}

void RocketChatAccount::loadAppCategories()
{
    if (mAppsCategoriesModel->wasFilled()) {
        return;
    }

    auto job = new RocketChatRestApi::AppCategoriesJob(this);
    restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppCategoriesJob::appCategoriesDone, this, [this](const QJsonArray &replyArray) {
        QList<AppsCategoryInfo> appsCategoryInfoList;
        const auto replyArrayCount{replyArray.count()};
        appsCategoryInfoList.reserve(replyArrayCount);
        // qDebug() << " array" << replyArray;
        for (int i = 0; i < replyArrayCount; ++i) {
            const QJsonObject obj = replyArray.at(i).toObject();
            // qDebug() << " obj" << obj;
            AppsCategoryInfo info;
            info.parseAppsCategoryInfo(obj);
            if (info.isValid()) {
                appsCategoryInfoList.append(std::move(info));
                // qDebug() << " info " << info;
            }
        }
        mAppsCategoriesModel->setAppsCategories(appsCategoryInfoList);
        loadInstalledApps();
    });
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start AppCategoriesJob";
    }
}

void RocketChatAccount::loadInstalledApps()
{
    auto job = new RocketChatRestApi::AppInstalledJob(this);
    restApi()->initializeRestApiJob(job);

    connect(job, &RocketChatRestApi::AppInstalledJob::appInstalledDone, this, [this](const QJsonArray &replyArray) {
        for (int i = 0; i < replyArray.count(); ++i) {
            const QJsonObject obj = replyArray.at(i).toObject();
            // qDebug() << "RocketChatAccount::loadInstalledApps obj" << obj;
            AppsMarketPlaceInstalledInfo info;
            info.parseInstalledAppsMarketPlaceInfo(obj);
            if (info.isValid()) {
                mAppsMarketPlaceModel->addInstalledInfo(info);
            }
        }
        Q_EMIT appsMarkPlaceLoadDone();
    });
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start AppInstalledJob";
    }
}

void RocketChatAccount::loadAppMarketPlace()
{
    if (mAppsMarketPlaceModel->wasFilled()) {
        Q_EMIT appsMarkPlaceLoadDone();
        return;
    }
    auto job = new RocketChatRestApi::AppMarketPlaceJob(this);
    job->setIsAdminUser(isAdministrator());
    restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppMarketPlaceJob::appMarketPlaceDone, this, [this](const QJsonArray &replyArray) {
        QList<AppsMarketPlaceInfo> listAppsMarketPlaceInfo;
        const auto replyArrayCount{replyArray.count()};
        listAppsMarketPlaceInfo.reserve(replyArrayCount);
        for (int i = 0; i < replyArrayCount; ++i) {
            const QJsonObject obj = replyArray.at(i).toObject();
            // qDebug() << " obj" << obj;
            AppsMarketPlaceInfo info;
            info.parseAppsMarketPlaceInfo(obj);
            if (info.isValid()) {
                listAppsMarketPlaceInfo.append(std::move(info));
                // qDebug() << " info " << info;
            }
        }
        mAppsMarketPlaceModel->setAppsMarketPlaceInfos(listAppsMarketPlaceInfo);
        loadAppCount();
    });
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start AppMarketPlaceJob";
    }
}

void RocketChatAccount::slotVerifyKeysDone()
{
    // TODO reactivate it when we will have full support
#if USE_E2E_SUPPORT
    Q_EMIT needToSaveE2EPassword();
    Q_EMIT needToDecryptE2EPassword();
    // TODO verify it!!!!!
    setE2EPasswordMustBeDecrypt(true);
    // TODO verify if we must decode it
#endif
}

MemoryManager *RocketChatAccount::memoryManager() const
{
    return mMemoryManager;
}

void RocketChatAccount::slotCleanRoomHistory()
{
    mRoomModel->cleanRoomHistory();
}

void RocketChatAccount::streamNotifyUserOtrEnd(const QByteArray &roomId, const QByteArray &userId)
{
    ddp()->streamNotifyUserOtrEnd(QString::fromLatin1(roomId), QString::fromLatin1(userId));
}

void RocketChatAccount::parseMethodRequested(const QJsonObject &obj, DDPClient::MethodRequestedType type)
{
    switch (type) {
    case DDPClient::MethodRequestedType::Unknown:
        qCWarning(RUQOLA_LOG) << "DDPClient::MethodRequestedType::Unknown it's a bug!!!! ";
        break;
    case DDPClient::MethodRequestedType::PublicSettings:
        processPublicsettings(obj);
        break;
    case DDPClient::MethodRequestedType::UpdatePublicsettings:
        processUpdatePublicsettings(obj);
        break;
    case DDPClient::MethodRequestedType::ChangeDefaultStatus:
        displayLogInfo(QByteArrayLiteral("Change Default Status"), obj);
        break;
    case DDPClient::MethodRequestedType::OpenRoom:
        openRoom(obj);
        break;
    case DDPClient::MethodRequestedType::OpenDirectRoom:
        openDirectChannel(obj);
        break;
    case DDPClient::MethodRequestedType::CreateJitsiConfCall:
        createJitsiConfCall(obj);
        break;
    case DDPClient::MethodRequestedType::InputUserChannelAutocompleteThread:
        inputUserChannelAutocompleteThread(obj);
        break;
    case DDPClient::MethodRequestedType::InputUserChannelAutocomplete:
        inputUserChannelAutocomplete(obj);
        break;
    case DDPClient::MethodRequestedType::OtrEnd:
        otrEnd(obj);
        break;
    case DDPClient::MethodRequestedType::Enable2fa:
        enable2fa(obj);
        break;
    case DDPClient::MethodRequestedType::RegenerateCodes2fa:
        regenerateCodes2fa(obj);
        break;
    case DDPClient::MethodRequestedType::Disable2fa:
        disable2fa(obj);
        break;
    case DDPClient::MethodRequestedType::ValidateTempToken2fa:
        validateTempToken2fa(obj);
        break;
    case DDPClient::MethodRequestedType::UpdateCustomSound:
        displayLogInfo(QByteArrayLiteral("Update Custom Sound"), obj);
        break;
    case DDPClient::MethodRequestedType::DeleteCustomSound:
        displayLogInfo(QByteArrayLiteral("Delete Custom Sound"), obj);
        break;
    case DDPClient::MethodRequestedType::BlockUser:
        displayLogInfo(QByteArrayLiteral("Block User"), obj);
        break;
    case DDPClient::MethodRequestedType::UnBlockUser:
        displayLogInfo(QByteArrayLiteral("UnBlock User"), obj);
        break;
    case DDPClient::MethodRequestedType::AdminStatus:
        displayLogInfo(QByteArrayLiteral("Admin Status"), obj);
        break;
    case DDPClient::MethodRequestedType::ListCustomSounds:
        listCustomSounds(obj);
        break;
    case DDPClient::MethodRequestedType::BannerDismiss:
        displayLogInfo(QByteArrayLiteral("Banner Dismiss"), obj);
        break;
    case DDPClient::MethodRequestedType::GetLicenseModules:
        licenseGetModules(obj);
        break;
    case DDPClient::MethodRequestedType::VideoConferenceCall:
        displayLogInfo(QByteArrayLiteral("Video Conference Call"), obj);
        break;
    case DDPClient::MethodRequestedType::VideoConferenceRejected:
        displayLogInfo(QByteArrayLiteral("Video Conference Rejected"), obj);
        break;
    case DDPClient::MethodRequestedType::VideoConferenceAccepted:
        displayLogInfo(QByteArrayLiteral("Video Conference Accepted"), obj);
        break;
    case DDPClient::MethodRequestedType::VideoConferenceConfirmed:
        displayLogInfo(QByteArrayLiteral("Video Conference Confirmed"), obj);
        break;
    case DDPClient::MethodRequestedType::PublicsettingsAdministrator:
        processPublicsettingsAdministrator(obj);
        break;
    case DDPClient::MethodRequestedType::PrivatesettingsAdministrator:
        processPrivatesettingsAdministrator(obj);
        break;
    case DDPClient::MethodRequestedType::PermissionsAdministrator:
        processPermissionsAdministrator(obj);
        break;
    case DDPClient::MethodRequestedType::RoomsParsing:
        roomsParsing(obj);
        break;
    case DDPClient::MethodRequestedType::GetsubscriptionParsing:
        getsubscriptionParsing(obj);
        break;
    case DDPClient::MethodRequestedType::Unsubscribe:
        displayLogInfo(QByteArrayLiteral("Unsubscribe Method"), obj);
        break;
    case DDPClient::MethodRequestedType::ShowDebug:
        displayLogInfo(QByteArrayLiteral("Empty call back"), obj);
        break;
    case DDPClient::MethodRequestedType::GetThreadMessages: {
        // Nothing
        break;
    }
    }
}

void RocketChatAccount::displayLogInfo(const QByteArray &ba, const QJsonObject &obj)
{
    if (mRuqolaLogger) {
        mRuqolaLogger->dataReceived(ba + ": " + QJsonDocument(obj).toJson());
    } else {
        qCWarning(RUQOLA_LOG) << "empty_callback " << obj;
    }
}

void RocketChatAccount::processPublicsettingsAdministrator(const QJsonObject &obj)
{
    Q_EMIT publicSettingLoaded(obj);
    displayLogInfo(QByteArrayLiteral("Administrator Public Settings"), obj);
}

void RocketChatAccount::processPrivatesettingsAdministrator(const QJsonObject &obj)
{
    Q_EMIT privateSettingLoaded(obj);
    displayLogInfo(QByteArrayLiteral("Administrator Private Settings"), obj);
}

void RocketChatAccount::processPermissionsAdministrator(const QJsonObject &obj)
{
    Q_EMIT permissionSettingLoaded(obj);
    displayLogInfo(QByteArrayLiteral("Administrator Permissions"), obj);
}

void RocketChatAccount::roomsParsing(const QJsonObject &root)
{
    const QJsonObject obj = root.value("result"_L1).toObject();
    RoomModel *model = roomModel();

    // qDebug() << " doc " << doc;

    // QJsonArray removed = obj.value("remove"_L1).toArray();
    // qDebug() << " rooms_parsing: room removed *************************************************" << removed;
    const QJsonArray updated = obj.value("update"_L1).toArray();
    // qDebug() << " rooms_parsing: updated  *******************************************************: "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject roomJson = updated.at(i).toObject();
        const QString roomType = roomJson.value("t"_L1).toString();
        if (mRuqolaLogger) {
            QJsonDocument d;
            d.setObject(roomJson);

            mRuqolaLogger->dataReceived(QByteArrayLiteral("Rooms:") + d.toJson());
        }
        // TODO: why checking the room type?
        if (roomType == QLatin1Char('c') // Chat
            || roomType == QLatin1Char('p') /*Private chat*/
            || roomType == QLatin1Char('d') /*Direct chat*/) {
            // let's be extra safe around crashes
            if (loginStatus() == AuthenticationManager::LoggedIn) {
                model->updateRoom(roomJson);
            }
        }
    }
}

void RocketChatAccount::getsubscriptionParsing(const QJsonObject &root)
{
    // qCDebug(RUQOLA_MESSAGE_LOG) << " getsubscription_parsing " << root;
    const QJsonObject obj = root.value("result"_L1).toObject();
    RoomModel *model = roomModel();

    // qDebug() << " doc " << doc;

    const QJsonArray removed = obj.value("remove"_L1).toArray();
    if (!removed.isEmpty()) {
        // TODO implement it.
        qDebug() << " room removed " << removed;
    }

    const QJsonArray updated = obj.value("update"_L1).toArray();
    // qDebug() << " updated : "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();

        const QString roomType = room.value("t"_L1).toString();
        if (mRuqolaLogger) {
            QJsonDocument d;
            d.setObject(room);

            mRuqolaLogger->dataReceived(QByteArrayLiteral("Rooms subscriptions:") + d.toJson());
        }
        if (roomType == QLatin1Char('c') // Chat
            || roomType == QLatin1Char('p') // Private chat
            || roomType == QLatin1Char('d')) { // Direct chat
            // let's be extra safe around crashes
            if (loginStatus() == AuthenticationManager::LoggedIn) {
                model->addRoom(room);
            }
        } else if (roomType == QLatin1Char('l')) { // Live chat
            qCDebug(RUQOLA_LOG) << "Live Chat not implemented yet";
        }
    }
    // We need to load all room after get subscription to update parameters
    QJsonObject params;
    // TODO use timeStamp too
    params["$date"_L1] = QJsonValue(0); // get ALL rooms we've ever seen
    // Add timestamp https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-rooms

    ddp()->getRooms(params);

    initializeAccount();
}

void RocketChatAccount::licenseGetModules(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("License GetModule"), root);
    const QJsonArray obj = root.value("result"_L1).toArray();
    parseLicenses(obj);
}

void RocketChatAccount::listCustomSounds(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("list custom sounds"), root);
    const QJsonArray obj = root.value("result"_L1).toArray();
    parseCustomSounds(obj);
}

void RocketChatAccount::validateTempToken2fa(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("Validate Temp Token 2FA"), root);
    const QJsonObject obj = root.value("result"_L1).toObject();
    totpVerify(obj);
}

void RocketChatAccount::disable2fa(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("Disable 2FA"), root);
    totpDisabledVerify(root);
}

void RocketChatAccount::regenerateCodes2fa(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("Regenerate Codes 2FA"), root);
    const QJsonObject obj = root.value("result"_L1).toObject();
    // TODO
    qDebug() << " regenerateCodes_2fa " << root;
}

void RocketChatAccount::enable2fa(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("Enable 2FA"), root);

    const QJsonObject obj = root.value("result"_L1).toObject();
    generate2FaTotp(obj);
}

void RocketChatAccount::otrEnd(const QJsonObject &root)
{
    qDebug() << "otr_end  " << root;
    displayLogInfo(QByteArrayLiteral("Otr End"), root);
}

void RocketChatAccount::inputUserChannelAutocomplete(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("Input channel/User autocomplete"), root);
    const QJsonObject obj = root.value("result"_L1).toObject();
    inputTextManager()->inputTextCompleter(obj);
}

void RocketChatAccount::inputUserChannelAutocompleteThread(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("Input channel/User autocomplete thread dialog"), root);
    const QJsonObject obj = root.value("result"_L1).toObject();

    inputThreadMessageTextManager()->inputTextCompleter(obj);
}

void RocketChatAccount::createJitsiConfCall(const QJsonObject &root)
{
    displayLogInfo(QByteArrayLiteral("Create Jitsi Conf Call"), root);
}

void RocketChatAccount::openDirectChannel(const QJsonObject &root)
{
    const QJsonObject obj = root.value("result"_L1).toObject();
    qDebug() << " void open_direct_channel(const QJsonObject &root, RocketChatAccount *account)" << obj;
    if (!obj.isEmpty()) {
        const QByteArray rid = obj.value("rid"_L1).toString().toLatin1();
        if (!rid.isEmpty()) {
            initializeDirectChannel(rid);
        }
    }
    displayLogInfo(QByteArrayLiteral("Open Direct channel"), obj);
}

void RocketChatAccount::openRoom(const QJsonObject &obj)
{
    displayLogInfo(QByteArrayLiteral("Open Room"), obj);
}

void RocketChatAccount::processPublicsettings(const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    parsePublicSettings(obj, false);

    displayLogInfo(QByteArrayLiteral("Public Settings"), obj);
}

void RocketChatAccount::processUpdatePublicsettings(const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    // Update it.
    parsePublicSettings(obj, true);

    displayLogInfo(QByteArrayLiteral("Update Public Settings"), obj);
}

#include "moc_rocketchataccount.cpp"
