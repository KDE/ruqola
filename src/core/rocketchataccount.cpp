/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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

#include "rocketchataccount.h"
#include "authenticationmanager.h"
#include "config-ruqola.h"
#include "downloadappslanguages/downloadappslanguagesmanager.h"
#include "emoticons/emojimanager.h"
#include "emoticons/unicodeemoticonmanager.h"
#include "fileattachments.h"
#include "managerdatapaths.h"
#include "messagequeue.h"
#include "model/autotranslatelanguagesmodel.h"
#include "model/commandsmodel.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/discussionsmodel.h"
#include "model/emoticoncustommodel.h"
#include "model/emoticonfiltermodel.h"
#include "model/emoticonmodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include "model/inputcompletermodel.h"
#include "model/listmessagesmodelfilterproxymodel.h"
#include "model/loginmethodmodel.h"
#include "model/messagemodel.h"
#include "model/roomfilterproxymodel.h"
#include "model/roommodel.h"
#include "model/searchchannelfilterproxymodel.h"
#include "model/searchchannelmodel.h"
#include "model/searchmessagefilterproxymodel.h"
#include "model/searchmessagemodel.h"
#include "model/statusmodel.h"
#include "model/threadmessagemodel.h"
#include "model/usercompleterfilterproxymodel.h"
#include "model/usercompletermodel.h"
#include "model/usersforroommodel.h"
#include "model/usersmodel.h"
#include "otrmanager.h"
#include "rocketchatbackend.h"
#include "rocketchatcache.h"
#include "ruqola.h"
#include "ruqola_commands_debug.h"
#include "ruqola_debug.h"
#include "ruqola_notification_debug.h"
#include "ruqola_typing_notification_debug.h"
#include "ruqolaglobalconfig.h"
#include "ruqolalogger.h"
#include "typingnotification.h"

#include "channelcounterinfo.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "discussions.h"
#include "listmessages.h"
#include "managechannels.h"
#include "messagecache.h"
#include "receivetypingnotificationmanager.h"

#include <KLocalizedString>
#include <KNotification>
#include <QDesktopServices>
#include <QTimer>
#include <channelgroupbasejob.h>

#if HAVE_NETWORKMANAGER
#include <NetworkManagerQt/Manager>
#endif

#if HAVE_SOLID
#include <Solid/Power>
#endif

#include <plugins/pluginauthentication.h>
#include <plugins/pluginauthenticationinterface.h>

#include "channels/channelopenjob.h"
#include "groups/groupopenjob.h"
#include "users/setstatusjob.h"
#include "users/usersautocompletejob.h"

#define USE_REASTAPI_JOB 1

RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)
    : QObject(parent)
    , mAccountRoomSettings(new AccountRoomSettings)
    , mMessageCache(new MessageCache(this))
    , mManageChannels(new ManageChannels(this))
{
    qCDebug(RUQOLA_LOG) << " RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)" << accountFileName;
    // create an unique file for each account
    loadSettings(accountFileName);
    if (!qEnvironmentVariableIsEmpty("RUQOLA_LOGFILE")) {
        mRuqolaLogger = new RuqolaLogger(mSettings->accountName());
    }

    mServerConfigInfo = new ServerConfigInfo(this, this);
    // Create it before loading settings
    mLoginMethodModel = new LoginMethodModel(this);

    mInputTextManager = new InputTextManager(this, this);
    mInputTextManager->setObjectName(QStringLiteral("mInputTextManager"));
    connect(mInputTextManager,
            &InputTextManager::completionRequested,
            this,
            [this](const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type) {
                inputAutocomplete(pattern, exceptions, type, false);
            });

    mInputThreadMessageTextManager = new InputTextManager(this, this);
    mInputThreadMessageTextManager->setObjectName(QStringLiteral("mInputThreadMessageTextManager"));
    connect(mInputThreadMessageTextManager,
            &InputTextManager::completionRequested,
            this,
            [this](const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type) {
                inputAutocomplete(pattern, exceptions, type, true);
            });

    mRuqolaServerConfig = new RuqolaServerConfig;
    mReceiveTypingNotificationManager = new ReceiveTypingNotificationManager(this);

    initializeAuthenticationPlugins();

    mRocketChatBackend = new RocketChatBackend(this, this);

    // Load list of unicode emoticon
    UnicodeEmoticonManager::self();

    // After loadSettings
    mEmojiManager = new EmojiManager(this);
    mEmojiManager->setServerUrl(mSettings->serverUrl());

    mEmoticonCustomModel = new EmoticonCustomModel(this);

    mEmoticonModel = new EmoticonModel(this);
    mEmoticonModel->setUnicodeEmoticons(mEmojiManager->unicodeEmojiList());
    mInputTextManager->setEmoticonModel(mEmoticonModel);
    mInputThreadMessageTextManager->setEmoticonModel(mEmoticonModel);

    mCommandsModel = new CommandsModel(this);

    mEmoticonFilterModel = new EmoticonFilterModel(this);
    mEmoticonFilterModel->setSourceModel(mEmoticonModel);

    mEmoticonFilterModel->emoticonCategoriesModel()->setCategories(mEmojiManager->categories());

    mOtrManager = new OtrManager(this);
    mRoomFilterProxyModel = new RoomFilterProxyModel(this);

    mUserCompleterModel = new UserCompleterModel(this);
    mUserCompleterFilterModelProxy = new UserCompleterFilterProxyModel(this);
    mUserCompleterFilterModelProxy->setSourceModel(mUserCompleterModel);

    mSearchChannelModel = new SearchChannelModel(this);
    mSearchChannelFilterProxyModel = new SearchChannelFilterProxyModel(this);
    mSearchChannelFilterProxyModel->setSourceModel(mSearchChannelModel);

    mSearchMessageModel = new SearchMessageModel(QString(), this, nullptr, this);
    mSearchMessageFilterProxyModel = new SearchMessageFilterProxyModel(mSearchMessageModel, this);

    mFilesModelForRoom = new FilesForRoomModel(this, this);
    mFilesModelForRoom->setObjectName(QStringLiteral("filesmodelforrooms"));
    mFilesForRoomFilterProxyModel = new FilesForRoomFilterProxyModel(mFilesModelForRoom, this);
    mFilesForRoomFilterProxyModel->setObjectName(QStringLiteral("filesforroomfiltermodelproxy"));

    mDiscussionsModel = new DiscussionsModel(this);
    mDiscussionsModel->setObjectName(QStringLiteral("discussionsmodel"));
    mDiscussionsFilterProxyModel = new DiscussionsFilterProxyModel(mDiscussionsModel, this);
    mDiscussionsFilterProxyModel->setObjectName(QStringLiteral("discussionsfilterproxymodel"));

    mThreadMessageModel = new ThreadMessageModel(QString(), this, nullptr, this);
    mThreadMessageModel->setObjectName(QStringLiteral("threadmessagemodel"));

    mListMessageModel = new ListMessagesModel(QString(), this, nullptr, this);
    mListMessageModel->setObjectName(QStringLiteral("listmessagemodel"));

    mListMessagesFilterProxyModel = new ListMessagesModelFilterProxyModel(mListMessageModel, this);
    mListMessagesFilterProxyModel->setObjectName(QStringLiteral("listmessagesfiltermodelproxy"));

    mAutoTranslateLanguagesModel = new AutotranslateLanguagesModel(this);
    mAutoTranslateLanguagesModel->setObjectName(QStringLiteral("autotranslatelanguagesmodel"));

    mStatusModel = new StatusModel(this);
    mRoomModel = new RoomModel(this, this);
    connect(mRoomModel, &RoomModel::needToUpdateNotification, this, &RocketChatAccount::slotNeedToUpdateNotification);
    connect(mRoomModel, &RoomModel::roomNeedAttention, this, &RocketChatAccount::slotRoomNeedAttention);
    mRoomFilterProxyModel->setSourceModel(mRoomModel);
    mUserModel = new UsersModel(this);
    connect(mUserModel, &UsersModel::userStatusChanged, this, &RocketChatAccount::updateUserModel);
    mMessageQueue = new MessageQueue(this, this);
    mTypingNotification = new TypingNotification(this);
    mCache = new RocketChatCache(this, this);

    mDownloadAppsLanguagesManager = new DownloadAppsLanguagesManager(this);
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
            logOut();
            slotReconnectToServer();
        }
    });
#endif
    connect(mManageChannels, &ManageChannels::selectRoomByRoomIdRequested, this, &RocketChatAccount::selectRoomByRoomIdRequested);
    connect(mManageChannels, &ManageChannels::selectRoomByRoomNameRequested, this, &RocketChatAccount::selectRoomByRoomNameRequested);
    connect(mManageChannels, &ManageChannels::missingChannelPassword, this, &RocketChatAccount::missingChannelPassword);
    connect(mManageChannels, &ManageChannels::openArchivedRoom, this, &RocketChatAccount::openArchivedRoom);
}

RocketChatAccount::~RocketChatAccount()
{
    delete mCache;
    mCache = nullptr;

    delete mRuqolaServerConfig;
    delete mRuqolaLogger;
    delete mAccountRoomSettings;
}

Room::TeamRoomInfo RocketChatAccount::roomFromTeamId(const QString &teamId) const
{
    return mRoomModel->roomFromTeamId(teamId);
}

void RocketChatAccount::updateUserModel(const User &user)
{
    if (hasOldSubscriptionSupport()) {
        userStatusChanged(user);
    }
}

void RocketChatAccount::removeSettings()
{
    mSettings->removeSettings();
}

void RocketChatAccount::loadSettings(const QString &accountFileName)
{
    delete mSettings;
    mSettings = new RocketChatAccountSettings(accountFileName, this);
    connect(mSettings, &RocketChatAccountSettings::serverURLChanged, this, &RocketChatAccount::serverUrlChanged);
    connect(mSettings, &RocketChatAccountSettings::userIdChanged, this, &RocketChatAccount::userIdChanged);
    connect(mSettings, &RocketChatAccountSettings::userNameChanged, this, &RocketChatAccount::userNameChanged);
    connect(mSettings, &RocketChatAccountSettings::passwordChanged, this, &RocketChatAccount::passwordChanged);
}

void RocketChatAccount::slotRoomNeedAttention()
{
    qCDebug(RUQOLA_NOTIFICATION_LOG) << " emit alert"
                                     << " account name: " << accountName();
    Q_EMIT roomNeedAttention();
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

SearchChannelFilterProxyModel *RocketChatAccount::searchChannelFilterProxyModel() const
{
    return mSearchChannelFilterProxyModel;
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

QString RocketChatAccount::userStatusIconFileName(const QString &name)
{
    return mUserModel->userStatusIconFileName(name);
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

RoomFilterProxyModel *RocketChatAccount::roomFilterProxyModel() const
{
    return mRoomFilterProxyModel;
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

MessageModel *RocketChatAccount::messageModelForRoom(const QString &roomID)
{
    return mRoomModel->messageModel(roomID);
}

void RocketChatAccount::changeShowOriginalMessage(const QString &roomId, const QString &messageId, bool showOriginal)
{
    MessageModel *model = mRoomModel->messageModel(roomId);
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
        connect(mRestApi, &RocketChatRestApi::Connection::loadEmojiCustomDone, this, &RocketChatAccount::loadEmoji);
        connect(mRestApi, &RocketChatRestApi::Connection::channelMembersDone, this, &RocketChatAccount::parseUsersForRooms);
        connect(mRestApi, &RocketChatRestApi::Connection::channelFilesDone, this, &RocketChatAccount::slotChannelFilesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::channelRolesDone, this, &RocketChatAccount::slotChannelRolesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::searchMessageDone, this, &RocketChatAccount::slotSearchMessages);
        connect(mRestApi, &RocketChatRestApi::Connection::failed, this, &RocketChatAccount::jobFailed);
        connect(mRestApi, &RocketChatRestApi::Connection::spotlightDone, this, &RocketChatAccount::slotSplotLightDone);
        connect(mRestApi, &RocketChatRestApi::Connection::directoryDone, this, &RocketChatAccount::slotDirectoryDone);
        connect(mRestApi, &RocketChatRestApi::Connection::getThreadMessagesDone, this, &RocketChatAccount::slotGetThreadMessagesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::getDiscussionsDone, this, &RocketChatAccount::slotGetDiscussionsListDone);
        connect(mRestApi, &RocketChatRestApi::Connection::channelListDone, this, &RocketChatAccount::slotChannelListDone);
        connect(mRestApi, &RocketChatRestApi::Connection::markAsReadDone, this, &RocketChatAccount::slotMarkAsReadDone);
        connect(mRestApi, &RocketChatRestApi::Connection::postMessageDone, this, &RocketChatAccount::slotPostMessageDone);

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
        connect(mRestApi, &RocketChatRestApi::Connection::getSnippetedMessagesDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::SnipperedMessages);
        });
        connect(mRestApi, &RocketChatRestApi::Connection::getStarredMessagesDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::StarredMessages);
        });

        connect(mRestApi, &RocketChatRestApi::Connection::getSupportedLanguagesDone, this, &RocketChatAccount::slotGetSupportedLanguagesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::usersPresenceDone, this, &RocketChatAccount::slotUsersPresenceDone);
        connect(mRestApi, &RocketChatRestApi::Connection::usersAutocompleteDone, this, &RocketChatAccount::slotUserAutoCompleterDone);
        connect(mRestApi,
                &RocketChatRestApi::Connection::roomsAutoCompleteChannelAndPrivateDone,
                this,
                &RocketChatAccount::slotRoomsAutoCompleteChannelAndPrivateDone);
        connect(mRestApi, &RocketChatRestApi::Connection::listCommandsDone, this, &RocketChatAccount::slotListCommandDone);
        connect(mRestApi, &RocketChatRestApi::Connection::registerUserDone, this, &RocketChatAccount::slotRegisterUserDone);
        connect(mRestApi, &RocketChatRestApi::Connection::channelGetCountersDone, this, &RocketChatAccount::slotChannelGetCountersDone);
        connect(mRestApi, &RocketChatRestApi::Connection::customUserStatusDone, this, &RocketChatAccount::slotCustomUserStatusDone);
        connect(mRestApi, &RocketChatRestApi::Connection::roomExportDone, this, &RocketChatAccount::slotRoomExportDone);
        connect(mRestApi, &RocketChatRestApi::Connection::permissionListAllDone, this, &RocketChatAccount::slotPermissionListAllDone);
        connect(mRestApi, &RocketChatRestApi::Connection::usersSetPreferencesDone, this, &RocketChatAccount::slotUsersSetPreferencesDone);
        connect(mRestApi, &RocketChatRestApi::Connection::uploadProgress, this, &RocketChatAccount::slotUploadProgress);
        mRestApi->setServerUrl(mSettings->serverUrl());
        mRestApi->setRestApiLogger(mRuqolaLogger);
        mCache->setRestApiConnection(mRestApi);
    }
    return mRestApi;
}

void RocketChatAccount::leaveRoom(const QString &roomId, Room::RoomType channelType)
{
    switch (channelType) {
    case Room::RoomType::Private:
        restApi()->leaveGroups(roomId);
        break;
    case Room::RoomType::Channel:
        restApi()->leaveChannel(roomId);
        break;
    case Room::RoomType::Direct:
    case Room::RoomType::Unknown:
        qCWarning(RUQOLA_LOG) << " unsupport leave room for type " << channelType;
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
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
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
        qCWarning(RUQOLA_LOG) << " unsupport delete for type " << channelType;
        break;
    }
}

void RocketChatAccount::openDirectChannel(const QString &username)
{
    // Laurent for the moment I didn't find a restapi method for it
    // TODO verify username vs userId
    //#ifdef USE_REASTAPI_JOB
    //    restApi()->openDirectMessage(username);
    //#else
    qDebug() << "Open direct conversation channel with" << username;
    ddp()->openDirectChannel(username);
    //#endif
}

void RocketChatAccount::createNewChannel(const RocketChatRestApi::CreateRoomInfo &info, bool privateRoom)
{
    if (!info.name.trimmed().isEmpty()) {
        if (privateRoom) {
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
    restApi()->listEmojiCustom();
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

void RocketChatAccount::changeDefaultStatus(int index, const QString &messageStatus)
{
    setDefaultStatus(mStatusModel->status(index), messageStatus);
}

void RocketChatAccount::loadEmoji(const QJsonObject &obj)
{
    mEmojiManager->loadCustomEmoji(obj);
    mEmoticonModel->setCustomEmojiList(mEmojiManager->customEmojiList());
    mEmoticonCustomModel->setCustomEmojiList(mEmojiManager->customEmojiList());
}

void RocketChatAccount::deleteMessage(const QString &messageId, const QString &roomId)
{
    restApi()->deleteMessage(roomId, messageId);
}

void RocketChatAccount::insertCompleterUsers()
{
    userCompleterModel()->insertUsers(rocketChatBackend()->users());
}

void RocketChatAccount::roomsAutocomplete(const QString &searchText, const QString &exception)
{
    RocketChatRestApi::RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo info;
    info.name = searchText;
    info.exception = exception;
    restApi()->roomsAutocomplete(info);
}

void RocketChatAccount::userAutocomplete(const QString &searchText, const QString &exception)
{
    userCompleterModel()->clear();
    if (mRuqolaServerConfig->hasAtLeastVersion(2, 4, 0)) {
        if (!searchText.isEmpty()) {
            RocketChatRestApi::UsersAutocompleteJob::UsersAutocompleterInfo info;
            info.pattern = searchText;
            info.exception = exception;
            restApi()->usersAutocomplete(info);
        }
    } else {
        // Clear before to create new search
        rocketChatBackend()->clearUsersList();
        if (!searchText.isEmpty()) {
            // Avoid to show own user
            QString addUserNameToException;
            if (exception.isEmpty()) {
                addUserNameToException = userName();
            } else {
                addUserNameToException = exception + QLatin1Char(',') + userName();
            }
            ddp()->userAutocomplete(searchText, addUserNameToException);
        }
    }
}

void RocketChatAccount::membersInRoom(const QString &roomId, Room::RoomType channelType)
{
    restApi()->membersInRoom(roomId, Room::roomFromRoomType(channelType));
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

MessageModel *RocketChatAccount::threadMessageModel() const
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

EmoticonFilterModel *RocketChatAccount::emoticonFilterModel() const
{
    return mEmoticonFilterModel;
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

void RocketChatAccount::slotChannelRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    Room *room = mRoomModel->findRoom(channelInfo.identifier);
    if (room) {
        Roles r;
        r.parseRole(obj);
        room->setRolesForRooms(r);
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
    const QVector<User> users = User::parseUsersList(obj);
    mUserCompleterModel->insertUsers(users);
}

void RocketChatAccount::slotRoomsAutoCompleteChannelAndPrivateDone(const QJsonObject &obj)
{
    qDebug() << " void RocketChatAccount::slotRoomsAutoCompleteChannelAndPrivateDone(const QJsonObject &obj)" << obj;
    // TODO
}

User::PresenceStatus RocketChatAccount::presenceStatus() const
{
    return mPresenceStatus;
}

AccountRoomSettings *RocketChatAccount::accountRoomSettings() const
{
    return mAccountRoomSettings;
}

ListMessagesModelFilterProxyModel *RocketChatAccount::listMessagesFilterProxyModel() const
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

EmoticonCustomModel *RocketChatAccount::emoticonCustomModel() const
{
    return mEmoticonCustomModel;
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

bool RocketChatAccount::hasStarredMessagesSupport() const
{
    return mRuqolaServerConfig->hasAtLeastVersion(2, 3, 0);
}

void RocketChatAccount::getStarredMessages(const QString &roomId)
{
    if (hasStarredMessagesSupport()) {
        mListMessageModel->clear();
        mListMessageModel->setRoomId(roomId);
        mListMessageModel->setLoadMoreListMessagesInProgress(true);
        restApi()->getStarredMessages(roomId);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::getStarredMessages is not supported before server 2.3.0";
    }
}

bool RocketChatAccount::hasInviteUserSupport() const
{
    return mRuqolaServerConfig->hasAtLeastVersion(2, 4, 0);
}

bool RocketChatAccount::hasSnippetedMessagesSupport() const
{
    return mRuqolaServerConfig->hasAtLeastVersion(2, 3, 0);
}

void RocketChatAccount::getSnippetedMessages(const QString &roomId)
{
    if (hasSnippetedMessagesSupport()) {
        mListMessageModel->clear();
        mListMessageModel->setRoomId(roomId);
        mListMessageModel->setLoadMoreListMessagesInProgress(true);
        restApi()->getSnippetedMessages(roomId);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::getSnippetedMessages is not supported before server 2.3.0";
    }
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
        if (hasStarredMessagesSupport()) {
            getStarredMessages(roomId);
        }
        break;
    case ListMessagesModel::SnipperedMessages:
        if (hasSnippetedMessagesSupport()) {
            getSnippetedMessages(roomId);
        }
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

    // TODO
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
                if (hasStarredMessagesSupport()) {
                    restApi()->getStarredMessages(roomId, offset, qMin(50, mListMessageModel->total() - offset));
                }
                break;
            case ListMessagesModel::SnipperedMessages:
                if (hasSnippetedMessagesSupport()) {
                    restApi()->getSnippetedMessages(roomId, offset, qMin(50, mListMessageModel->total() - offset));
                }
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
        mSearchMessageModel->setLoadSearchMessageInProgress(true);
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
    mSearchMessageModel->setLoadSearchMessageInProgress(false);
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

void RocketChatAccount::uploadFile(const QString &roomId,
                                   const QString &description,
                                   const QString &messageText,
                                   const QUrl &fileUrl,
                                   const QString &threadMessageId)
{
    restApi()->uploadFile(roomId, description, messageText, fileUrl, threadMessageId);
}

void RocketChatAccount::reportMessage(const QString &messageId, const QString &message)
{
    restApi()->reportMessage(messageId, message);
}

void RocketChatAccount::getThreadMessages(const QString &threadMessageId)
{
    mListMessageModel->clear();
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
    case AudioNotifications:
        restApi()->audioNotifications(roomId, newValue.toString());
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
    }
}

void RocketChatAccount::parsePublicSettings(const QJsonObject &obj)
{
    mRuqolaServerConfig->parsePublicSettings(obj);
    fillOauthModel();
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

SearchMessageFilterProxyModel *RocketChatAccount::searchMessageFilterProxyModel() const
{
    return mSearchMessageFilterProxyModel;
}

SearchMessageModel *RocketChatAccount::searchMessageModel() const
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
            mLstInfos.append(info);
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
    // qDebug() << "void RocketChatAccount::setAccountName(const QString &servername)"<<accountname;
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

void RocketChatAccount::deleteCustomUserStatus(const QJsonArray &replyArray)
{
    qDebug() << " void RocketChatAccount::deleteCustomUserStatus(const QJsonObject &replyObject)" << replyArray;
    mCustomUserStatuses.deleteCustomUserStatuses(replyArray);
    Q_EMIT customUserStatusChanged();
}

void RocketChatAccount::deleteUser(const QJsonArray &replyArray)
{
    qDebug() << " void RocketChatAccount::deleteUser(const QJsonObject &replyObject)" << replyArray;
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

void RocketChatAccount::setUseLdap(bool ldap)
{
    settings()->setUseLdap(ldap);
}

bool RocketChatAccount::useLdap() const
{
    return settings()->useLdap();
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

QUrl RocketChatAccount::attachmentUrl(const QString &url)
{
    return mCache->attachmentUrl(url);
}

QString RocketChatAccount::loadMessagesHistory(const QString &roomID, qint64 numberOfMessages)
{
    MessageModel *roomMessageModel = messageModelForRoom(roomID);
    if (roomMessageModel->rowCount() > numberOfMessages) {
        return roomMessageModel->messageIdFromIndex(roomMessageModel->rowCount() - numberOfMessages);
    }
    if (roomMessageModel) {
        const auto realNumberOfMessages = numberOfMessages - roomMessageModel->rowCount() + 2;
        if (realNumberOfMessages > 0) {
            QJsonArray params;
            params.append(QJsonValue(roomID));
            // Load history
            const qint64 endDateTime = roomMessageModel->lastTimestamp();

            QJsonObject dateObjectEnd;
            dateObjectEnd[QStringLiteral("$date")] = QJsonValue(endDateTime);

            params.append(dateObjectEnd);

            params.append(realNumberOfMessages); // Max number of messages to load;

            QJsonObject dateObjectStart;
            qDebug() << "roomModel->lastTimestamp()" << roomMessageModel->lastTimestamp() << " ROOMID " << roomID << " numberOfMessages " << numberOfMessages;
            qDebug() << " params" << params;
            ddp()->loadHistory(params);
        }
    }
    return {};
}

void RocketChatAccount::loadHistory(const QString &roomID, bool initial, qint64 timeStep)
{
    MessageModel *roomModel = messageModelForRoom(roomID);
    if (roomModel) {
        // TODO add autotest for it !
        QJsonArray params;
        params.append(QJsonValue(roomID));
        // Load history
        const qint64 endDateTime = roomModel->lastTimestamp();
        if (initial || roomModel->isEmpty()) {
            params.append(QJsonValue(QJsonValue::Null));
            params.append(QJsonValue(50)); // Max number of messages to load;
            QJsonObject dateObject;
            // qDebug() << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID;
            dateObject[QStringLiteral("$date")] = QJsonValue(endDateTime);
            params.append(dateObject);
        } else if (timeStep != 0) {
            QJsonObject dateObjectEnd;
            dateObjectEnd[QStringLiteral("$date")] = QJsonValue(endDateTime);

            // qDebug() << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  << QDateTime::fromMSecsSinceEpoch(startDateTime) << "
            // ROOMID" << roomID;
            params.append(dateObjectEnd);

            params.append(QJsonValue(175)); // Max number of messages to load;

            QJsonObject dateObjectStart;
            qDebug() << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID << " timeStep " << timeStep;
            // dateObjectStart[QStringLiteral("$date")] = QJsonValue(timeStep);
            // params.append(dateObjectStart);
            qDebug() << " params" << params;
        } else {
            const qint64 startDateTime = roomModel->generateNewStartTimeStamp(endDateTime);
            QJsonObject dateObjectEnd;
            dateObjectEnd[QStringLiteral("$date")] = QJsonValue(endDateTime);

            // qDebug() << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  << QDateTime::fromMSecsSinceEpoch(startDateTime) << "
            // ROOMID" << roomID;
            params.append(dateObjectEnd);

            params.append(QJsonValue(50)); // Max number of messages to load;

            QJsonObject dateObjectStart;
            // qDebug() << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID;
            dateObjectStart[QStringLiteral("$date")] = QJsonValue(startDateTime);
            params.append(dateObjectStart);
        }
        ddp()->loadHistory(params);
    } else {
        qCWarning(RUQOLA_LOG) << "Room is not found " << roomID;
    }
}

void RocketChatAccount::setServerVersion(const QString &version)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::setServerVersion(const QString &version)" << version;
    mRuqolaServerConfig->setServerVersion(version);
    Q_EMIT serverVersionChanged();
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

bool RocketChatAccount::allowMessageSnippetingEnabled() const
{
    return mRuqolaServerConfig->serverConfigFeatureTypes() & RuqolaServerConfig::ServerConfigFeatureType::AllowMessageSnippeting;
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

bool RocketChatAccount::teamEnabled() const
{
    return mRuqolaServerConfig->hasAtLeastVersion(3, 13, 0) && hasPermission(QStringLiteral("create-team"));
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

void RocketChatAccount::setSortUnreadOnTop(bool b)
{
    if (settings()->setShowUnreadOnTop(b)) {
        mRoomFilterProxyModel->invalidate();
        Q_EMIT sortUnreadOnTopChanged();
    }
}

bool RocketChatAccount::sortUnreadOnTop() const
{
    return settings()->showUnreadOnTop();
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
        qCWarning(RUQOLA_LOG) << " unsupport kickUser room for type " << channelType;
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
        qCWarning(RUQOLA_LOG) << " unsupport get roles room for type " << channelType;
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
        qCWarning(RUQOLA_LOG) << " unsupport changeRoles room for type " << channelType;
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
    return (message.timeStamp() + ruqolaServerConfig()->blockEditingMessageInMinutes() * 60 * 1000) > QDateTime::currentMSecsSinceEpoch();
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
    return (message.timeStamp() + ruqolaServerConfig()->blockDeletingMessageInMinutes() * 60 * 1000) > QDateTime::currentMSecsSinceEpoch();
}

void RocketChatAccount::parseOtr(const QJsonArray &contents)
{
    qCWarning(RUQOLA_LOG) << " NOT IMPLEMENTED YET";
    // const Otr t = mOtrManager->parseOtr(contents);
    // qDebug() << " void RocketChatAccount::parseOtr(const QJsonArray &contents)"<<t;
    // TODO add notification ?
}

void RocketChatAccount::sendNotification(const QJsonArray &contents)
{
    Utils::NotificationInfo info = Utils::parseNotification(contents);

    const QString iconFileName = mCache->avatarUrlFromCacheOnly(info.senderId);
    // qDebug() << " iconFileName"<<iconFileName << " sender " << sender;
    QPixmap pix;
    if (!iconFileName.isEmpty()) {
        const QUrl url = QUrl::fromLocalFile(iconFileName);
        // qDebug() << "url.toLocalFile()"<<url.toLocalFile();
        const bool loaded = pix.load(url.toLocalFile().remove(QStringLiteral("file://")), "JPEG");
        // qDebug() << " load pixmap : "<< loaded;
        // qDebug() << " pix " << pix.isNull();
        Q_UNUSED(loaded)
        info.pixmap = pix;
    }
    if (!info.isValid()) {
        qCWarning(RUQOLA_LOG) << " Info is invalid ! " << contents;
    }
    Q_EMIT notification(info);
}

void RocketChatAccount::inputAutocomplete(const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type, bool threadDialog)
{
    // TODO look at for restapi support.
    switch (type) {
    case InputTextManager::CompletionForType::Channel:
        ddp()->inputChannelAutocomplete(pattern, exceptions, threadDialog);
        break;
    case InputTextManager::CompletionForType::User:
        ddp()->inputUserAutocomplete(pattern, exceptions, threadDialog);
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
    if (!hasOldSubscriptionSupport()) {
        mUserModel->addUser(user);
    }
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
            mCache->updateAvatar(info);
            Q_EMIT avatarWasChanged(info);
        } else if (obj.contains(QLatin1String("rid"))) {
            const QString roomId = obj[QLatin1String("rid")].toString();
            const QString etag = obj[QLatin1String("etag")].toString();
            qDebug() << "need to update room avatar " << accountName() << "room" << roomId;
            Utils::AvatarInfo info;
            info.avatarType = Utils::AvatarType::Room;
            info.etag = etag; // Etag
            info.identifier = roomId; // roomId
            mCache->updateAvatar(info);
            Q_EMIT avatarWasChanged(info);
        } else {
            qWarning() << "avatar changed but missing roomId or userId. It seems to be a regression in RC? " << contents;
        }
    }

    // TODO parse "QJsonObject({"args":[{"username":"foo"}],"eventName":"updateAvatar"})"
}

void RocketChatAccount::rolesChanged(const QJsonArray &contents)
{
    for (int i = 0; i < contents.count(); ++i) {
        const QJsonObject obj = contents.at(i).toObject();
        const QString scope = obj[QLatin1String("scope")].toString();
        Room *room = mRoomModel->findRoom(scope);
        if (room) {
            room->updateRoles(obj);
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
    mListMessageModel->clear();
    mListMessageModel->setRoomId(roomId);
    restApi()->getThreadsList(roomId, onlyUnread);
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

bool RocketChatAccount::hasOldSubscriptionSupport() const
{
    return !mRuqolaServerConfig->hasAtLeastVersion(3, 0, 0);
}

void RocketChatAccount::getSupportedLanguages()
{
    if (autoTranslateEnabled()) {
        restApi()->getSupportedLanguagesMessages();
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
        user.parseUserRestApi(userJson);
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
    // TODO: delay this more and more like RC+ ?
    QTimer::singleShot(mDelayReconnect, this, [this]() {
        qCDebug(RUQOLA_LOG) << "Attempting to reconnect after the server disconnected us: " << accountName();
        mDelayReconnect += 1000;
        tryLogin();
    });
}

void RocketChatAccount::usersPresence()
{
    restApi()->usersPresence();
}

void RocketChatAccount::customUsersStatus()
{
    if (mRuqolaServerConfig->hasAtLeastVersion(2, 4, 0)) {
        restApi()->customUserStatus();
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::customUserStatus is not supported before server 2.4.0";
    }
}

void RocketChatAccount::initializeAccount()
{
    listEmojiCustom();

    // load when necessary
    usersPresence();
    if (hasAutotranslateSupport()) {
        getSupportedLanguages();
    }
    // Force set online.
    // TODO don't reset message status !
    if (RuqolaGlobalConfig::self()->setOnlineAccounts()) {
        ddp()->setDefaultStatus(User::PresenceStatus::PresenceOnline);
    }
    customUsersStatus();

    Q_EMIT accountInitialized();
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
    restApi()->listCommands();
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
    // qDebug() << "RocketChatBackend::parseOwnInfoDown " << replyObject;
    mOwnUser.parseOwnUserInfo(replyObject);
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
    Q_EMIT ownInfoChanged();
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

void RocketChatAccount::addMessage(const QJsonObject &replyObject, bool useRestApi, bool temporaryMessage)
{
    const QString roomId = replyObject.value(QLatin1String("rid")).toString();
    if (!roomId.isEmpty()) {
        MessageModel *messageModel = messageModelForRoom(roomId);
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
        messageModel->addMessages({m});
        // qDebug() << " m " << m;
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

void RocketChatAccount::exportMessages(const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &info)
{
    restApi()->exportMessages(info);
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

void RocketChatAccount::slotUploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info)
{
    Q_EMIT uploadProgress(info);
}

MessageCache *RocketChatAccount::messageCache() const
{
    return mMessageCache;
}

void RocketChatAccount::slotUpdateCustomUserStatus()
{
    mStatusModel->updateCustomStatus(mCustomUserStatuses.customUserses());
    Q_EMIT updateStatusComboBox();
}

bool RocketChatAccount::hideRoles() const
{
    return ownUser().ownUserPreferences().hideRoles();
}

bool RocketChatAccount::hideAvatars() const
{
    return ownUser().ownUserPreferences().hideAvatars();
}
