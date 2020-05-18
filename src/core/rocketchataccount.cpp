/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "model/roommodel.h"
#include "roomwrapper.h"
#include "typingnotification.h"
#include "model/usersmodel.h"
#include "ruqola_debug.h"
#include "ruqola.h"
#include "messagequeue.h"
#include "rocketchatbackend.h"
#include "model/roomfilterproxymodel.h"
#include "ruqolalogger.h"
#include "ruqolaserverconfig.h"
#include "model/usercompletermodel.h"
#include "model/usercompleterfilterproxymodel.h"
#include "model/statusmodel.h"
#include "ruqolaglobalconfig.h"
#include "ruqola_typing_notification_debug.h"
#include "utils.h"
#include "rocketchatcache.h"
#include "fileattachments.h"
#include "emoticons/emojimanager.h"
#include "model/emoticonfiltermodel.h"
#include "otrmanager.h"
#include "inputtextmanager.h"
#include "model/usersforroommodel.h"
#include "model/filesforroommodel.h"
#include "model/searchchannelfilterproxymodel.h"
#include "model/searchchannelmodel.h"
#include "model/loginmethodmodel.h"
#include "model/inputcompletermodel.h"
#include "model/searchmessagemodel.h"
#include "model/searchmessagefilterproxymodel.h"
#include "model/discussionsmodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/listmessagesmodel.h"
#include "model/threadmessagemodel.h"
#include "model/listmessagesmodelfilterproxymodel.h"
#include "model/autotranslatelanguagesmodel.h"
#include "model/commandsmodel.h"
#include "model/emoticonmodel.h"
#include "managerdatapaths.h"
#include "authenticationmanager.h"

#include "ddpapi/ddpclient.h"
#include "ddpapi/ddpauthenticationmanager.h"
#include "discussions.h"
#include "receivetypingnotificationmanager.h"
#include "restapirequest.h"
#include "serverconfiginfo.h"
#include "listmessages.h"

#include <QDesktopServices>
#include <QTimer>

#include <plugins/pluginauthentication.h>
#include <plugins/pluginauthenticationinterface.h>

#include "users/setstatusjob.h"
#include "users/usersautocompletejob.h"

#define USE_REASTAPI_JOB 1

RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)
    : QObject(parent)
{
    mAccountRoomSettings = new AccountRoomSettings;
    qCDebug(RUQOLA_LOG) << " RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)"<<accountFileName;
    //create an unique file for each account
    loadSettings(accountFileName);
    if (!qEnvironmentVariableIsEmpty("RUQOLA_LOGFILE")) {
        mRuqolaLogger = new RuqolaLogger(mSettings->accountName());
    }

    mServerConfigInfo = new ServerConfigInfo(this, this);
    //Create it before loading settings
    mLoginMethodModel = new LoginMethodModel(this);
    mInputTextManager = new InputTextManager(this);
    connect(mInputTextManager, &InputTextManager::completionRequested, this, &RocketChatAccount::inputAutocomplete);

    mInputThreadMessageTextManager = new InputTextManager(this);
    connect(mInputThreadMessageTextManager, &InputTextManager::completionRequested, this, &RocketChatAccount::inputAutocomplete);

    mRuqolaServerConfig = new RuqolaServerConfig;
    mReceiveTypingNotificationManager = new ReceiveTypingNotificationManager(this);

    initializeAuthenticationPlugins();

    mRocketChatBackend = new RocketChatBackend(this, this);

    //After loadSettings
    mEmojiManager = new EmojiManager(this);
    mEmojiManager->setServerUrl(mSettings->serverUrl());

    mEmoticonModel = new EmoticonModel(this);
    mEmoticonModel->setEmoticons(mEmojiManager->unicodeEmojiList());
    mInputTextManager->setEmoticonModel(mEmoticonModel);

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
    mRoomFilterProxyModel->setSourceModel(mRoomModel);
    mUserModel = new UsersModel(this);
    connect(mUserModel, &UsersModel::userStatusChanged, this, &RocketChatAccount::updateUserModel);
    mMessageQueue = new MessageQueue(this, this); //TODO fix mem leak !
    mTypingNotification = new TypingNotification(this);
    mCache = new RocketChatCache(this, this);
    connect(mCache, &RocketChatCache::fileDownloaded, this, &RocketChatAccount::fileDownloaded);
    connect(mTypingNotification, &TypingNotification::informTypingStatus, this, &RocketChatAccount::slotInformTypingStatus);
    QTimer::singleShot(0, this, &RocketChatAccount::clearModels);
}

RocketChatAccount::~RocketChatAccount()
{
    delete mCache;
    mCache = nullptr;

    delete mRuqolaServerConfig;
    delete mRuqolaLogger;
    delete mAccountRoomSettings;
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
    connect(mSettings, &RocketChatAccountSettings::userIDChanged, this, &RocketChatAccount::userIDChanged);
    connect(mSettings, &RocketChatAccountSettings::userNameChanged, this, &RocketChatAccount::userNameChanged);
    connect(mSettings, &RocketChatAccountSettings::passwordChanged, this, &RocketChatAccount::passwordChanged);
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
    //Try to send queue message
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

UsersForRoomFilterProxyModel *RocketChatAccount::usersForRoomFilterProxyModel(const QString &roomId) const
{
    return mRoomModel->usersForRoomFilterProxyModel(roomId);
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

Room *RocketChatAccount::getRoom(const QString &roomId)
{
    return mRoomModel->findRoom(roomId);
}

DiscussionsFilterProxyModel *RocketChatAccount::discussionsFilterProxyModel() const
{
    return mDiscussionsFilterProxyModel;
}

RoomWrapper *RocketChatAccount::roomWrapper(const QString &roomId)
{
    return mRoomModel->findRoomWrapper(roomId);
}

MessageModel *RocketChatAccount::messageModelForRoom(const QString &roomID)
{
    return mRoomModel->messageModel(roomID);
}

void RocketChatAccount::changeDisplayAttachment(const QString &roomId, const QString &messageId, bool displayAttachment)
{
    MessageModel *model = mRoomModel->messageModel(roomId);
    if (model) {
        model->changeDisplayAttachment(messageId, displayAttachment);
    } else {
        qCWarning(RUQOLA_LOG) << "impossible to find room: " << roomId;
    }
    //TODO
}

void RocketChatAccount::changeShowOriginalMessage(const QString &roomId, const QString &messageId, bool showOriginal)
{
    MessageModel *model = mRoomModel->messageModel(roomId);
    if (model) {
        model->changeShowOriginalMessage(messageId, showOriginal);
    } else {
        qCWarning(RUQOLA_LOG) << "impossible to find room: " << roomId;
    }
    //TODO
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
    restApi()->reactOnMessage(messageId, emoji, shouldReact);
}

void RocketChatAccount::sendMessage(const QString &roomID, const QString &message)
{
    restApi()->postMessage(roomID, message);
}

void RocketChatAccount::updateMessage(const QString &roomID, const QString &messageId, const QString &message)
{
    restApi()->updateMessage(roomID, messageId, message);
}

void RocketChatAccount::replyOnThread(const QString &roomID, const QString &threadMessageId, const QString &message)
{
    restApi()->sendMessage(roomID, message, QString(), threadMessageId);
}

QString RocketChatAccount::avatarUrlFromDirectChannel(const QString &rid)
{
    return mCache->avatarUrl(Utils::userIdFromDirectChannel(rid, userID()));
}

void RocketChatAccount::deleteFileMessage(const QString &roomId, const QString &fileId, const QString &channelType)
{
    ddp()->deleteFileMessage(roomId, fileId, channelType);
}

QString RocketChatAccount::avatarUrl(const QString &userId)
{
    return mCache->avatarUrl(userId);
}

void RocketChatAccount::insertAvatarUrl(const QString &userId, const QUrl &url)
{
    mCache->insertAvatarUrl(userId, url);
}

RocketChatRestApi::RestApiRequest *RocketChatAccount::restApi()
{
    if (!mRestApi) {
        mRestApi = new RocketChatRestApi::RestApiRequest(this);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::setChannelJoinDone, this, &RocketChatAccount::setChannelJoinDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::missingChannelPassword, this, &RocketChatAccount::missingChannelPassword);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::loadEmojiCustomDone, this, &RocketChatAccount::loadEmoji);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::openArchivedRoom, this, &RocketChatAccount::openArchivedRoom);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelMembersDone, this, &RocketChatAccount::parseUsersForRooms);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelFilesDone, this, &RocketChatAccount::slotChannelFilesDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelRolesDone, this, &RocketChatAccount::slotChannelRolesDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::searchMessageDone, this, &RocketChatAccount::slotSearchMessages);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::failed, this, &RocketChatAccount::jobFailed);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::spotlightDone, this, &RocketChatAccount::slotSplotLightDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getThreadMessagesDone, this, &RocketChatAccount::slotGetThreadMessagesDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getDiscussionsDone, this, &RocketChatAccount::slotGetDiscussionsListDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelListDone, this, &RocketChatAccount::slotChannelListDone);

        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getThreadsDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::ThreadsMessages);
        });
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelGetAllUserMentionsDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::MentionsMessages);
        });
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getPinnedMessagesDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::PinnedMessages);
        });
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getSnippetedMessagesDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::SnipperedMessages);
        });
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getStarredMessagesDone, this, [this](const QJsonObject &obj, const QString &roomId) {
            slotGetListMessagesDone(obj, roomId, ListMessagesModel::ListMessageType::StarredMessages);
        });

        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getSupportedLanguagesDone, this, &RocketChatAccount::slotGetSupportedLanguagesDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::usersPresenceDone, this, &RocketChatAccount::slotUsersPresenceDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::usersAutocompleteDone, this, &RocketChatAccount::slotUserAutoCompleterDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::roomsAutoCompleteChannelAndPrivateDone, this, &RocketChatAccount::slotRoomsAutoCompleteChannelAndPrivateDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::listCommandsDone, this, &RocketChatAccount::slotListCommandDone);
        mRestApi->setServerUrl(mSettings->serverUrl());
        mRestApi->setRestApiLogger(mRuqolaLogger);
    }
    return mRestApi;
}

void RocketChatAccount::leaveRoom(const QString &roomId, const QString &channelType)
{
    if (channelType == QLatin1Char('c')) {
        restApi()->leaveChannel(roomId);
    } else if (channelType == QLatin1Char('p')) {
        restApi()->leaveGroups(roomId);
    } else {
        qCWarning(RUQOLA_LOG) << " unsupport leave room for type " << channelType;
    }
}

void RocketChatAccount::hideRoom(const QString &roomId, const QString &channelType)
{
    restApi()->closeChannel(roomId, channelType);
}

DDPClient *RocketChatAccount::ddp()
{
    if (!mDdp) {
        mDdp = new DDPClient(this, this);
        connect(mDdp->authenticationManager(), &DDPAuthenticationManager::loginStatusChanged,
                this, &RocketChatAccount::loginStatusChangedSlot);
        connect(mDdp, &DDPClient::connectedChanged, this, &RocketChatAccount::connectedChanged);
        connect(mDdp, &DDPClient::changed, this, &RocketChatAccount::changed);
        connect(mDdp, &DDPClient::added, this, &RocketChatAccount::added);
        connect(mDdp, &DDPClient::removed, this, &RocketChatAccount::removed);
        connect(mDdp, &DDPClient::socketError, this, &RocketChatAccount::socketError);
        connect(mDdp, &DDPClient::disconnectedByServer, this, &RocketChatAccount::slotDisconnectedByServer);

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
    mDdp->authenticationManager()->logout();
}

void RocketChatAccount::clearAllUnreadMessages()
{
    for (int roomIdx = 0, nRooms = mRoomModel->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = mRoomModel->index(roomIdx);
        const auto roomId = roomModelIndex.data(RoomModel::RoomID).toString();
        const bool roomHasAlert = roomModelIndex.data(RoomModel::RoomAlert).toBool();
        if (roomHasAlert) {
            markRoomAsRead(roomId);
        }
    }
}

void RocketChatAccount::markRoomAsRead(const QString &roomId)
{
    restApi()->markRoomAsRead(roomId);
}

void RocketChatAccount::changeFavorite(const QString &roomId, bool checked)
{
    restApi()->markAsFavorite(roomId, checked);
}

void RocketChatAccount::openChannel(const QString &url, ChannelTypeInfo typeInfo)
{
    RocketChatRestApi::ChannelBaseJob::ChannelInfo info;
    switch (typeInfo) {
    case ChannelTypeInfo::RoomId:
        info.channelInfoType = RocketChatRestApi::ChannelBaseJob::ChannelInfoType::RoomId;
        break;
    case ChannelTypeInfo::RoomName:
        info.channelInfoType = RocketChatRestApi::ChannelBaseJob::ChannelInfoType::RoomName;
        break;
    }
    info.channelInfoIdentifier = url;
    qCDebug(RUQOLA_LOG) << "opening channel" << url;
    restApi()->channelJoin(info, QString());
}

void RocketChatAccount::setChannelJoinDone(const RocketChatRestApi::ChannelBaseJob::ChannelInfo &channelInfo)
{
    ddp()->subscribeRoomMessage(channelInfo.channelInfoIdentifier);
    //FIXME room is not added yet...
    switch (channelInfo.channelInfoType) {
    case RocketChatRestApi::ChannelBaseJob::ChannelInfoType::Unknown:
        qCWarning(RUQOLA_LOG) << "setChannelJoinDone : RocketChatRestApi::ChannelBaseJob::ChannelInfoType::Unknown";
        break;
    case RocketChatRestApi::ChannelBaseJob::ChannelInfoType::RoomId:
        Q_EMIT selectRoomByRoomIdRequested(channelInfo.channelInfoIdentifier);
        break;
    case RocketChatRestApi::ChannelBaseJob::ChannelInfoType::RoomName:
        Q_EMIT selectRoomByRoomNameRequested(channelInfo.channelInfoIdentifier);
        break;
    }
}

void RocketChatAccount::openArchivedRoom(const RocketChatRestApi::ChannelBaseJob::ChannelInfo &channelInfo)
{
    //TODO
}

void RocketChatAccount::joinJitsiConfCall(const QString &roomId)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::joinJitsiConfCall(const QString &roomId)"<<roomId;
    const QString hash = QString::fromLatin1(QCryptographicHash::hash((mRuqolaServerConfig->uniqueId() + roomId).toUtf8(), QCryptographicHash::Md5).toHex());
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    const QString scheme = QStringLiteral("org.jitsi.meet://");
#else
    const QString scheme = QStringLiteral("https://");
#endif
    const QString url = scheme + mRuqolaServerConfig->jitsiMeetUrl() + QLatin1Char('/') + mRuqolaServerConfig->jitsiMeetPrefix() + hash;
    const QUrl clickedUrl = QUrl::fromUserInput(url);
    QDesktopServices::openUrl(clickedUrl);
}

void RocketChatAccount::eraseRoom(const QString &roomId, const QString &channelType)
{
    if (channelType == QLatin1Char('c')) {
        restApi()->channelDelete(roomId);
    } else if (channelType == QLatin1Char('p')) {
        restApi()->groupDelete(roomId);
    } else {
        qCWarning(RUQOLA_LOG) << " unsupport delete for type " << channelType;
    }
}

void RocketChatAccount::openDirectChannel(const QString &username)
{
    //Laurent for the moment I didn't find a restapi method for it
    //TODO verify username vs userId
//#ifdef USE_REASTAPI_JOB
//    restApi()->openDirectMessage(username);
//#else
    qDebug() << "Open direct conversation channel with" << username;
    ddp()->openDirectChannel(username);
//#endif
}

void RocketChatAccount::createNewChannel(const QString &name, bool readOnly, bool privateRoom, const QString &userNames, bool encryptedRoom, const QString &password, bool broadcast)
{
    //TODO use encrypted room
    //TODO use broadcast
    if (!name.trimmed().isEmpty()) {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        const QStringList lstUsers = userNames.split(QLatin1Char(','), QString::SkipEmptyParts);
#else
        const QStringList lstUsers = userNames.split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
        if (privateRoom) {
            //TODO add password ???
            restApi()->createGroups(name, readOnly, lstUsers);
        } else {
            restApi()->createChannels(name, readOnly, lstUsers, password);
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
    RocketChatRestApi::ChannelBaseJob::ChannelInfo info;
    info.channelInfoType = RocketChatRestApi::ChannelBaseJob::ChannelInfoType::RoomId;
    info.channelInfoIdentifier = roomId;
    restApi()->channelJoin(info, joinCode);
}

void RocketChatAccount::channelAndPrivateAutocomplete(const QString &pattern)
{
    if (pattern.isEmpty()) {
        searchChannelModel()->clear();
    } else {
        //Use restapi
        //Avoid to show own user
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
    restApi()->setUserStatus(userID(), type, messageStatus);
}

void RocketChatAccount::changeDefaultStatus(int index, const QString &messageStatus)
{
    setDefaultStatus(mStatusModel->status(index), messageStatus);
}

void RocketChatAccount::loadEmoji(const QJsonObject &obj)
{
    mEmojiManager->loadCustomEmoji(obj);
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
    if (mRuqolaServerConfig->hasAtLeastVersion(2, 4, 0)) {
        if (!searchText.isEmpty()) {
            RocketChatRestApi::UsersAutocompleteJob::UsersAutocompleterInfo info;
            info.pattern = searchText;
            info.exception = exception;
            restApi()->usersAutocomplete(info);
        }
    } else {
        //Clear before to create new search
        rocketChatBackend()->clearUsersList();
        if (!searchText.isEmpty()) {
            //Avoid to show own user
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

void RocketChatAccount::membersInRoom(const QString &roomId, const QString &roomType)
{
    restApi()->membersInRoom(roomId, roomType);
}

void RocketChatAccount::parseUsersForRooms(const QJsonObject &obj, const RocketChatRestApi::ChannelBaseJob::ChannelInfo &channelInfo)
{
    //FIXME channelInfo
    const QString channelInfoIdentifier = channelInfo.channelInfoIdentifier;
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(channelInfoIdentifier);
    if (usersModelForRoom) {
        usersModelForRoom->parseUsersForRooms(obj, mUserModel, true);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << channelInfoIdentifier;
    }
}

void RocketChatAccount::loadAutoCompleteChannel(const QJsonObject &obj)
{
    mSearchChannelModel->parseChannels(obj);
}

void RocketChatAccount::roomFiles(const QString &roomId, const QString &channelType)
{
    mFilesModelForRoom->initialize();
    restApi()->filesInRoom(roomId, channelType);
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

void RocketChatAccount::slotChannelRolesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelBaseJob::ChannelInfo &channelInfo)
{
    Room *room = mRoomModel->findRoom(channelInfo.channelInfoIdentifier);
    if (room) {
        Roles r;
        r.parseRole(obj);
        room->setRolesForRooms(r);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << channelInfo.channelInfoIdentifier;
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
    //TODO
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

void RocketChatAccount::slotSplotLightDone(const QJsonObject &obj)
{
    //qDebug() << " void RocketChatAccount::slotSplotLightDone(const QJsonObject &obj)"<<obj;
    //If empty ! show empty list
    loadAutoCompleteChannel(obj);
}

void RocketChatAccount::slotChannelListDone(const QJsonObject &obj)
{
    //qDebug() << " void RocketChatAccount::slotChannelListDone(const QJsonObject &obj)" << obj;
    mSearchChannelModel->parseAllChannels(obj);
}

void RocketChatAccount::slotChannelFilesDone(const QJsonObject &obj, const RocketChatRestApi::ChannelBaseJob::ChannelInfo &channelInfo)
{
    //TODO fixme channelinfo
    //qDebug() << " slotChannelFilesDone(const QJsonObject &obj, const QString &roomId)" << roomId << " obj " << obj;
    if (mFilesModelForRoom->roomId() != channelInfo.channelInfoIdentifier) {
        mFilesModelForRoom->parseFileAttachments(obj, channelInfo.channelInfoIdentifier);
    } else {
        mFilesModelForRoom->addMoreFileAttachments(obj);
    }
    mFilesModelForRoom->setLoadMoreFilesInProgress(false);
}

void RocketChatAccount::loadMoreUsersInRoom(const QString &roomId, const QString &channelType)
{
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    const int offset = usersModelForRoom->usersCount();
    if (offset < usersModelForRoom->total()) {
        restApi()->membersInRoom(roomId, channelType, offset, qMin(50, usersModelForRoom->total() - offset));
    }
}

void RocketChatAccount::getMentionsMessages(const QString &roomId)
{
    mListMessageModel->clear();
    mListMessageModel->setRoomId(roomId);
    mListMessageModel->setLoadMoreListMessagesInProgress(true);
    restApi()->channelGetAllUserMentions(roomId);
}

void RocketChatAccount::getPinnedMessages(const QString &roomId)
{
    if (hasPinnedMessagesSupport()) {
        mListMessageModel->clear();
        mListMessageModel->setLoadMoreListMessagesInProgress(true);
        mListMessageModel->setRoomId(roomId);
        restApi()->getPinnedMessages(roomId);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::getPinnedMessages is not supported before server 2.0.0";
    }
}

bool RocketChatAccount::hasPinnedMessagesSupport() const
{
    return mRuqolaServerConfig->hasAtLeastVersion(1, 4, 0);
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

void RocketChatAccount::loadMoreFileAttachments(const QString &roomId, const QString &channelType)
{
    if (!mFilesModelForRoom->loadMoreFilesInProgress()) {
        const int offset = mFilesModelForRoom->fileAttachments()->filesCount();
        if (offset < mFilesModelForRoom->fileAttachments()->total()) {
            mFilesModelForRoom->setLoadMoreFilesInProgress(true);
            restApi()->filesInRoom(roomId, channelType, offset, qMin(50, mFilesModelForRoom->fileAttachments()->total() - offset));
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
        mThreadMessageModel->addMessage(m);
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
        threadsInRoom(roomId);
        break;
    }
}

QUrl RocketChatAccount::urlForLink(const QString &link) const
{
    return mCache->urlForLink(link);
}

void RocketChatAccount::setNameChanged(const QJsonArray &array)
{
    qCWarning(RUQOLA_LOG) << "Need to implement: Users:NameChanged :" << array;

    //TODO
}

void RocketChatAccount::setOwnStatus(const User &user)
{
    userStatusChanged(user);
}

void RocketChatAccount::setUserStatusChanged(const QJsonArray &array)
{
    //qDebug() << "Account Name : " << accountName() << " status changed: " << array << " array " << array.count() << " array" << array.toVariantList();
    const auto list = array.toVariantList();
    for (const auto &var : list) {
        const auto userListArguments = var.toJsonArray().toVariantList();
        User user;
        user.parseUser(userListArguments);
        if (user.isValid()) {
            userStatusChanged(user);
            //qDebug() << " user status changed " << user;
        }
    }
}

void RocketChatAccount::loadMoreListMessages(const QString &roomId)
{
    if (!mListMessageModel->loadMoreListMessagesInProgress()) {
        mListMessageModel->setLoadMoreListMessagesInProgress(true);
        const int offset = mListMessageModel->rowCount();
        if (offset < mListMessageModel->total()) {
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
                restApi()->channelGetAllUserMentions(roomId, offset, qMin(50, mListMessageModel->total() - offset));
                break;
            case ListMessagesModel::ThreadsMessages:
                restApi()->getThreadsList(roomId, offset, qMin(50, mListMessageModel->total() - offset));
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
    //TODO use restapi
    ddp()->createJitsiConfCall(roomId);
    joinJitsiConfCall(roomId);
}

void RocketChatAccount::addUserToRoom(const QString &userId, const QString &roomId, const QString &channelType)
{
    if (channelType == QLatin1Char('c')) {
        restApi()->addUserInChannel(roomId, userId);
    } else if (channelType == QLatin1Char('p')) {
        restApi()->addUserInGroup(roomId, userId);
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

void RocketChatAccount::uploadFile(const QString &roomId, const QString &description, const QString &messageText, const QUrl &fileUrl, const QString &threadMessageId)
{
    restApi()->uploadFile(roomId, description, messageText, fileUrl, threadMessageId);
}

void RocketChatAccount::changeChannelSettings(const QString &roomId, RocketChatAccount::RoomInfoType infoType, const QVariant &newValue, const QString &channelType)
{
    switch (infoType) {
    case Announcement:
        if (channelType == QLatin1Char('c')) {
            restApi()->changeChannelAnnouncement(roomId, newValue.toString());
        } else if (channelType == QLatin1Char('p')) {
            restApi()->changeGroupsAnnouncement(roomId, newValue.toString());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change announcement for type " << channelType;
        }
        break;
    case Description:
        if (channelType == QLatin1Char('c')) {
            restApi()->changeChannelDescription(roomId, newValue.toString());
        } else if (channelType == QLatin1Char('p')) {
            restApi()->changeGroupsDescription(roomId, newValue.toString());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change description for type " << channelType;
        }
        break;
    case Name:
        if (channelType == QLatin1Char('c')) {
            restApi()->changeChannelName(roomId, newValue.toString());
        } else if (channelType == QLatin1Char('p')) {
            restApi()->changeGroupName(roomId, newValue.toString());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change name for type " << channelType;
        }
        break;
    case Topic:
        if (channelType == QLatin1Char('c')) {
            restApi()->changeChannelTopic(roomId, newValue.toString());
        } else if (channelType == QLatin1Char('p')) {
            restApi()->changeGroupsTopic(roomId, newValue.toString());
        } else {
            //TODO : change topic in direct channel
            qCWarning(RUQOLA_LOG) << " unsupport change topic for type " << channelType;
        }
        break;
    case ReadOnly:
        if (channelType == QLatin1Char('c')) {
            restApi()->changeChannelReadOnly(roomId, newValue.toBool());
        } else if (channelType == QLatin1Char('p')) {
            restApi()->changeGroupsReadOnly(roomId, newValue.toBool());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change readonly for type " << channelType;
        }
        break;
    case Archive:
        if (channelType == QLatin1Char('c')) {
            restApi()->archiveChannel(roomId, newValue.toBool());
        } else if (channelType == QLatin1Char('p')) {
            restApi()->archiveGroups(roomId, newValue.toBool());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport archiving for type " << channelType;
        }
        break;
    case RoomType:
        if (channelType == QLatin1Char('c')) {
            restApi()->setChannelType(roomId, newValue.toBool());
        } else if (channelType == QLatin1Char('p')) {
            restApi()->setGroupType(roomId, newValue.toBool());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport roomtype for type " << channelType;
        }
        break;
    case Encrypted:
        if (channelType == QLatin1Char('c')) {
            restApi()->changeChannelEncrypted(roomId, newValue.toBool());
        } else if (channelType == QLatin1Char('p')) {
            restApi()->changeGroupsEncrypted(roomId, newValue.toBool());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport encrypted mode for type " << channelType;
        }
        break;
    case Password:
        //FIXME channel type ???
        //restApi()->setJoinCodeChannel(roomId, newValue.toString());
        break;
    }
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

void RocketChatAccount::changeNotificationsSettings(const QString &roomId, RocketChatAccount::NotificationOptionsType notificationsType, const QVariant &newValue)
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
    //TODO change it when we change server
    //Clean up at the end.
    const QVector<PluginAuthentication *> lstPlugins = AuthenticationManager::self()->pluginsList();
    qCDebug(RUQOLA_LOG) <<" void RocketChatAccount::initializeAuthenticationPlugins()" << lstPlugins.count();
    if (lstPlugins.isEmpty()) {
        qCWarning(RUQOLA_LOG) <<" No plugins loaded. Please verify your installation.";
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
        //For the moment initialize default interface
        if (abstractPlugin->type() == AuthenticationManager::OauthType::Password) {
            mDefaultAuthenticationInterface = interface;
        }
        qCDebug(RUQOLA_LOG) << " plugin type " << abstractPlugin->type();
    }
    //TODO fill ??? or store QVector<AuthenticationInfo>
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
    //Initialize new account room
    //qDebug() << "void RocketChatAccount::setAccountName(const QString &servername)"<<accountname;
    loadSettings(ManagerDataPaths::self()->accountConfigFileName(accountname));
    settings()->setAccountName(accountname);
}

QString RocketChatAccount::accountName() const
{
    return settings()->accountName();
}

QString RocketChatAccount::userID() const
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

void RocketChatAccount::setUserID(const QString &userID)
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
    mCache->downloadFile(downloadFileUrl, localFile, false);
}

QUrl RocketChatAccount::attachmentUrl(const QString &url)
{
    return mCache->attachmentUrl(url);
}

void RocketChatAccount::loadHistory(const QString &roomID, const QString &channelType, bool initial)
{
    //TODO port to restapi
    Q_UNUSED(channelType);
    MessageModel *roomModel = messageModelForRoom(roomID);
    if (roomModel) {
        //TODO add autotest for it !
        QJsonArray params;
        params.append(QJsonValue(roomID));
        // Load history
        const qint64 endDateTime = roomModel->lastTimestamp();
        if (initial || roomModel->isEmpty()) {
            params.append(QJsonValue(QJsonValue::Null));
            params.append(QJsonValue(50)); // Max number of messages to load;
            QJsonObject dateObject;
            //qDebug() << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID;
            dateObject[QStringLiteral("$date")] = QJsonValue(endDateTime);
            params.append(dateObject);
        } else {
            const qint64 startDateTime = roomModel->generateNewStartTimeStamp(endDateTime);
            QJsonObject dateObjectEnd;
            dateObjectEnd[QStringLiteral("$date")] = QJsonValue(endDateTime);

            //qDebug() << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  << QDateTime::fromMSecsSinceEpoch(startDateTime) << " ROOMID" << roomID;
            params.append(dateObjectEnd);

            params.append(QJsonValue(50)); // Max number of messages to load;

            QJsonObject dateObjectStart;
            //qDebug() << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID;
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
    return mRuqolaServerConfig->otrEnabled();
}

bool RocketChatAccount::encryptedEnabled() const
{
    return mRuqolaServerConfig->encryptionEnabled();
}

bool RocketChatAccount::allowMessagePinningEnabled() const
{
    return mRuqolaServerConfig->allowMessagePinning();
}

bool RocketChatAccount::allowMessageSnippetingEnabled() const
{
    return mRuqolaServerConfig->allowMessageSnippeting();
}

bool RocketChatAccount::allowMessageStarringEnabled() const
{
    return mRuqolaServerConfig->allowMessageStarring();
}

bool RocketChatAccount::allowMessageDeletingEnabled() const
{
    return mRuqolaServerConfig->allowMessageDeleting();
}

bool RocketChatAccount::threadsEnabled() const
{
    return mRuqolaServerConfig->threadsEnabled();
}

bool RocketChatAccount::autoTranslateEnabled() const
{
    return mRuqolaServerConfig->autoTranslateEnabled();
}

bool RocketChatAccount::discussionEnabled() const
{
    return mRuqolaServerConfig->discussionEnabled();
}

ServerConfigInfo *RocketChatAccount::serverConfigInfo() const
{
    return mServerConfigInfo;
}

bool RocketChatAccount::jitsiEnabled() const
{
    return mRuqolaServerConfig->jitsiEnabled();
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

void RocketChatAccount::kickUser(const QString &roomId, const QString &userId, const QString &channelType)
{
    if (channelType == QLatin1Char('c')) {
        restApi()->channelKick(roomId, userId);
    } else if (channelType == QLatin1Char('p')) {
        restApi()->groupKick(roomId, userId);
    } else {
        qCWarning(RUQOLA_LOG) << " unsupport kickUser room for type " << channelType;
    }
}

void RocketChatAccount::rolesInRoom(const QString &roomId, const QString &channelType)
{
    if (channelType == QLatin1Char('c')) {
        restApi()->getChannelRoles(roomId);
    } else if (channelType == QLatin1Char('p')) {
        restApi()->getGroupRoles(roomId);
    } else if (channelType == QLatin1Char('d')) {
        //No a problem here.
    } else {
        qCWarning(RUQOLA_LOG) << " unsupport get roles room for type " << channelType;
    }
}

void RocketChatAccount::switchingToRoom(const QString &roomID)
{
    clearTypingNotification();
    checkInitializedRoom(roomID);
}

void RocketChatAccount::changeRoles(const QString &roomId, const QString &userId, const QString &channelType, RocketChatAccount::RoleType roleType)
{
    if (channelType == QLatin1Char('c')) {
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
    } else if (channelType == QLatin1Char('p')) {
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
    } else {
        qCWarning(RUQOLA_LOG) << " unsupport changeRoles room for type " << channelType;
    }
}

void RocketChatAccount::channelInfo(const QString &roomId)
{
    restApi()->channelInfo(roomId);
}

bool RocketChatAccount::allowEditingMessages() const
{
    return mRuqolaServerConfig->allowMessageEditing();
}

bool RocketChatAccount::isMessageEditable(const Message &message) const
{
    if (!allowEditingMessages()) {
        return false;
    }
    if (message.userId() != userID()) {
        return false;
    }
    return (message.timeStamp() + ruqolaServerConfig()->blockEditingMessageInMinutes() * 60 * 1000)
           > QDateTime::currentMSecsSinceEpoch();
}

bool RocketChatAccount::isMessageDeletable(const Message &message) const
{
    if (!mRuqolaServerConfig->allowMessageDeleting()) {
        return false;
    }
    if (message.userId() != userID()) {
        return false;
    }
    return (message.timeStamp() + ruqolaServerConfig()->blockDeletingMessageInMinutes() * 60 * 1000)
           > QDateTime::currentMSecsSinceEpoch();
}

void RocketChatAccount::parseOtr(const QJsonArray &contents)
{
    qCWarning(RUQOLA_LOG) << " NOT IMPLEMENTED YET";
    //const Otr t = mOtrManager->parseOtr(contents);
    //qDebug() << " void RocketChatAccount::parseOtr(const QJsonArray &contents)"<<t;
    //TODO add notification ?
}

void RocketChatAccount::sendNotification(const QJsonArray &contents)
{
    Utils::NotificationInfo info = Utils::parseNotification(contents);

    const QString iconFileName = mCache->avatarUrlFromCacheOnly(info.senderId);
    //qDebug() << " iconFileName"<<iconFileName << " sender " << sender;
    QPixmap pix;
    if (!iconFileName.isEmpty()) {
        const QUrl url = QUrl::fromLocalFile(iconFileName);
        //qDebug() << "url.toLocalFile()"<<url.toLocalFile();
        const bool loaded = pix.load(url.toLocalFile().remove(QStringLiteral("file://")), "JPEG");
        //qDebug() << " load pixmap : "<< loaded;
        //qDebug() << " pix " << pix.isNull();
        Q_UNUSED(loaded);
        info.pixmap = pix;
    }
    if (!info.isValid()) {
        qCWarning(RUQOLA_LOG) << " Info is invalid ! " << contents;
    }
    Q_EMIT notification(info);
}

void RocketChatAccount::inputAutocomplete(const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type)
{
    //TODO look at for restapi support.
    switch (type) {
    case InputTextManager::CompletionForType::Channel:
        ddp()->inputChannelAutocomplete(pattern, exceptions);
        break;
    case InputTextManager::CompletionForType::User:
        ddp()->inputUserAutocomplete(pattern, exceptions);
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
    if (user.userId() == userID()) {
        mPresenceStatus = Utils::presenceStatusFromString(user.status());
        statusModel()->setCurrentPresenceStatus(mPresenceStatus);
        Q_EMIT userStatusUpdated(mPresenceStatus, accountName());
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
    //TODO use restapi
    if (rid.isEmpty()) {
        qCWarning(RUQOLA_LOG) << " void RocketChatAccount::blockUser EMPTY rid ! block " << block;
    } else {
        //qDebug() << " void RocketChatAccount::blockUser userId " << userId << " block " << block << " rid " << rid << " own userdId" << userID();

        const QString userId = Utils::userIdFromDirectChannel(rid, userID());
        if (block) {
            ddp()->blockUser(rid, userId);
        } else {
            ddp()->unBlockUser(rid, userId);
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
        }
        loadHistory(r->roomId(), QString(), true /*initial loading*/);
    } else if (!r) {
        qWarning() << " Room " << roomId << " was no found! Need to open it";
        //openDirectChannel(roomId);
    }
    QMetaObject::invokeMethod(this, &RocketChatAccount::switchedRooms, Qt::QueuedConnection);
}

void RocketChatAccount::openDocumentation()
{
    QDesktopServices::openUrl(QUrl(QStringLiteral("help:/")));
}

void RocketChatAccount::avatarChanged(const QJsonArray &contents)
{
    qCWarning(RUQOLA_LOG) << "Need to implement updateAvatar :" << contents;

    //TODO parse "QJsonObject({"args":[{"username":"foo"}],"eventName":"updateAvatar"})"
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

void RocketChatAccount::createDiscussion(const QString &parentRoomId, const QString &discussionName, const QString &replyMessage, const QString &messageId, const QStringList &users)
{
    restApi()->createDiscussion(parentRoomId, discussionName, replyMessage, messageId, users);
}

void RocketChatAccount::threadsInRoom(const QString &roomId)
{
    mListMessageModel->clear();
    mListMessageModel->setRoomId(roomId);
    restApi()->getThreadsList(roomId);
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
    if (mRuqolaServerConfig->hasAtLeastVersion(1, 99, 0) && autoTranslateEnabled()) {
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
    if (mRuqolaServerConfig->hasAtLeastVersion(1, 99, 0)) {
        restApi()->autoTranslateSaveLanguageSettings(roomId, language);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::autoTranslateSaveLanguageSettings is not supported before server 2.0.0";
    }
}

void RocketChatAccount::autoTranslateSaveAutoTranslateSettings(const QString &roomId, bool autoTranslate)
{
    if (mRuqolaServerConfig->hasAtLeastVersion(1, 99, 0)) {
        restApi()->autoTranslateSaveAutoTranslateSettings(roomId, autoTranslate);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::autoTranslateSaveLanguageSettings is not supported before server 2.0.0";
    }
}

void RocketChatAccount::slotUsersPresenceDone(const QJsonObject &obj)
{
    //qDebug() << " void RocketChatAccount::slotUsersPresenceDone(const QJsonObject &obj)" << obj;
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

void RocketChatAccount::slotDisconnectedByServer()
{
    // This happens when we didn't react to pings for a while
    // (e.g. while stopped in gdb, or if network went down for a bit)
    // Let's try connecting in again
    // TODO: delay this more and more like RC+ ?
    QTimer::singleShot(100, this, [this]() {
        qCDebug(RUQOLA_LOG) << "Attempting to reconnect after the server disconnected us";
        // Do the parts of logOut() that don't actually try talking to the server
        mRoomModel->clear();
        delete mDdp;
        mDdp = nullptr;
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
    getListCommands();
    //load when necessary
    usersPresence();
    if (mRuqolaServerConfig->autoTranslateEnabled()) {
        getSupportedLanguages();
    }
    //Force set online.
    //TODO don't reset message status !
    if (RuqolaGlobalConfig::self()->setOnlineAccounts()) {
        ddp()->setDefaultStatus(User::PresenceStatus::PresenceOnline);
    }
    //customUsersStatus(); Only for test

    Q_EMIT accountInitialized();
}

void RocketChatAccount::getListCommands()
{
    restApi()->listCommands();
}

void RocketChatAccount::slotListCommandDone(const QJsonObject &obj)
{
    Commands commands;
    commands.parseCommands(obj);
    if (!mCommandsModel->commands().isEmpty()) { //Don't show command listview if we already have command (for example when we logout/login)
        QSignalBlocker blockSignal(mCommandsModel);
        mCommandsModel->setCommands(commands);
    } else {
        //Initialize it after loading otherwise we will see listview at startup
        mCommandsModel->setCommands(commands);
        mInputTextManager->setCommandModel(mCommandsModel);
    }
    //qDebug() << "slotListCommandDone " << obj;
}

bool RocketChatAccount::runCommand(const QString &msg, const QString &roomId)
{
    const RocketChatRestApi::RunCommandJob::RunCommandInfo info = RocketChatRestApi::RunCommandJob::parseString(msg, roomId);
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
    }

    Q_EMIT loginStatusChanged();
}
