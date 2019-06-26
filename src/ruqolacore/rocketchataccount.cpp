/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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
#include "utils.h"
#include "rocketchatcache.h"
#include "fileattachments.h"
#include "emoticons/emojimanager.h"
#include "model/emoticonmodel.h"
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
#include "model/threadsmodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/threadsfilterproxymodel.h"
#include "model/mentionsmodel.h"
#include "model/mentionsfilterproxymodel.h"
#include "managerdatapaths.h"
#include "authenticationmanager.h"

#include "ddpapi/ddpclient.h"
#include "discussions.h"
#include "receivetypingnotificationmanager.h"
#include "restapirequest.h"
#include "serverconfiginfo.h"
#include "threadmessages.h"
#include "threads.h"
#include "mentions.h"

#include <QDesktopServices>
#include <QRegularExpression>
#include <QTimer>

#include <plugins/pluginauthentication.h>
#include <plugins/pluginauthenticationinterface.h>

#define USE_REASTAPI_JOB 1

RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)
    : QObject(parent)
{
    qCDebug(RUQOLA_LOG) << " RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)"<<accountFileName;
    //create an unique file for each account
    if (!qEnvironmentVariableIsEmpty("RUQOLA_LOGFILE")) {
        mRuqolaLogger = new RuqolaLogger;
    }

    mServerConfigInfo = new ServerConfigInfo(this, this);
    //Create it before loading settings
    mLoginMethodModel = new LoginMethodModel(this);
    mInputTextManager = new InputTextManager(this, this);
    mRuqolaServerConfig = new RuqolaServerConfig;
    mReceiveTypingNotificationManager = new ReceiveTypingNotificationManager(this);

    initializeAuthenticationPlugins();

    loadSettings(accountFileName);

    mRocketChatBackend = new RocketChatBackend(this, this);

    mEmoticonModel = new EmoticonModel(this);
    //TODO

    //After loadSettings
    mEmojiManager = new EmojiManager(this);
    mEmojiManager->setServerUrl(mSettings->serverUrl());

    mEmoticonModel->setEmoticons(mEmojiManager->unicodeEmojiList());

    mOtrManager = new OtrManager(this);
    mRoomFilterProxyModel = new RoomFilterProxyModel(this);

    mUserCompleterModel = new UserCompleterModel(this);
    mUserCompleterFilterModelProxy = new UserCompleterFilterProxyModel(this);
    mUserCompleterFilterModelProxy->setSourceModel(mUserCompleterModel);

    mSearchChannelModel = new SearchChannelModel(this);
    mSearchChannelFilterProxyModel = new SearchChannelFilterProxyModel(this);
    mSearchChannelFilterProxyModel->setSourceModel(mSearchChannelModel);

    mSearchMessageModel = new SearchMessageModel(this);
    mSearchMessageFilterProxyModel = new SearchMessageFilterProxyModel(this);
    mSearchMessageFilterProxyModel->setSourceModel(mSearchMessageModel);

    mFilesModelForRoom = new FilesForRoomModel(this, this);
    mFilesModelForRoom->setObjectName(QStringLiteral("filesmodelforrooms"));
    mFilesForRoomFilterProxyModel = new FilesForRoomFilterProxyModel(this);
    mFilesForRoomFilterProxyModel->setObjectName(QStringLiteral("filesforroomfiltermodelproxy"));
    mFilesForRoomFilterProxyModel->setSourceModel(mFilesModelForRoom);

    mDiscussionsModel = new DiscussionsModel(this);
    mDiscussionsModel->setObjectName(QStringLiteral("discussionsmodel"));
    mDiscussionsFilterProxyModel = new DiscussionsFilterProxyModel(this);
    mDiscussionsFilterProxyModel->setObjectName(QStringLiteral("discussionsfilterproxymodel"));
    mDiscussionsFilterProxyModel->setSourceModel(mDiscussionsModel);

    mThreadsModel = new ThreadsModel(this);
    mThreadsModel->setObjectName(QStringLiteral("threadsmodel"));

    mThreadsFilterProxyModel = new ThreadsFilterProxyModel(this);
    mThreadsFilterProxyModel->setObjectName(QStringLiteral("threadsfiltermodelproxy"));
    mThreadsFilterProxyModel->setSourceModel(mThreadsModel);

    mMentionsModel = new MentionsModel(this, this);
    mMentionsModel->setObjectName(QStringLiteral("mentionsmodel"));

    mMentionsFilterProxyModel = new MentionsFilterProxyModel(this);
    mMentionsFilterProxyModel->setObjectName(QStringLiteral("mentionsfiltermodelproxy"));
    mMentionsFilterProxyModel->setSourceModel(mMentionsModel);

    mThreadMessageModel = new MessageModel(QString(), this, nullptr, this);
    mThreadMessageModel->setObjectName(QStringLiteral("threadmessagemodel"));

    mStatusModel = new StatusModel(this);
    mRoomModel = new RoomModel(this, this);
    connect(mRoomModel, &RoomModel::needToUpdateNotification, this, &RocketChatAccount::slotNeedToUpdateNotification);
    mRoomFilterProxyModel->setSourceModel(mRoomModel);
    mUserModel = new UsersModel(this);
    connect(mUserModel, &UsersModel::userStatusChanged, this, &RocketChatAccount::userStatusChanged);
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

ThreadsFilterProxyModel *RocketChatAccount::threadsFilterProxyModel() const
{
    return mThreadsFilterProxyModel;
}

RoomWrapper *RocketChatAccount::getRoomWrapper(const QString &roomId)
{
    return mRoomModel->findRoomWrapper(roomId);
}

MessageModel *RocketChatAccount::messageModelForRoom(const QString &roomID)
{
    return mRoomModel->messageModel(roomID);
}

QString RocketChatAccount::getUserCurrentMessage(const QString &roomId)
{
    return mRoomModel->inputMessage(roomId);
}

void RocketChatAccount::setUserCurrentMessage(const QString &message, const QString &roomId)
{
    mRoomModel->setInputMessage(roomId, message);
}

void RocketChatAccount::setInputTextChanged(const QString &str, int position)
{
    mInputTextManager->setInputTextChanged(str, position);
}

QString RocketChatAccount::replaceWord(const QString &newWord, const QString &str, int position)
{
    return mInputTextManager->replaceWord(newWord, str, position);
}

void RocketChatAccount::textEditing(const QString &roomId, const QString &str)
{
    mTypingNotification->setText(roomId, str);
}

void RocketChatAccount::reactOnMessage(const QString &messageId, const QString &emoji, bool shouldReact)
{
    restApi()->reactOnMessage(messageId, emoji, shouldReact);
}

void RocketChatAccount::replyToMessage(const QString &roomID, const QString &message, const QString &messageId)
{
    restApi()->postMessage(roomID, message);
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

void RocketChatAccount::insertAvatarUrl(const QString &userId, const QString &url)
{
    mCache->insertAvatarUrl(userId, url);
}

RocketChatRestApi::RestApiRequest *RocketChatAccount::restApi()
{
    if (!mRestApi) {
        mRestApi = new RocketChatRestApi::RestApiRequest(this);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::setChannelJoinDone, this, &RocketChatAccount::setChannelJoinDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::missingChannelPassword, this, &RocketChatAccount::missingChannelPassword);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::loadEmojiCustomDone, this, &RocketChatAccount::loadEmojiRestApi);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::openArchivedRoom, this, &RocketChatAccount::openArchivedRoom);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelMembersDone, this, &RocketChatAccount::parseUsersForRooms);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelFilesDone, this, &RocketChatAccount::slotChannelFilesDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelRolesDone, this, &RocketChatAccount::slotChannelRolesDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::searchMessageDone, this, &RocketChatAccount::slotSearchMessages);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::failed, this, &RocketChatAccount::jobFailed);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::spotlightDone, this, &RocketChatAccount::slotSplotLightDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getThreadMessagesDone, this, &RocketChatAccount::slotGetThreadMessagesDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getThreadsDone, this, &RocketChatAccount::slotGetThreadsListDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::getDiscussionsDone, this, &RocketChatAccount::slotGetDiscussionsListDone);
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::channelGetAllUserMentionsDone, this, &RocketChatAccount::slotGetAllUserMentionsDone);
        mRestApi->setServerUrl(mSettings->serverUrl());
        mRestApi->setRestApiLogger(mRuqolaLogger);
    }
    return mRestApi;
}

void RocketChatAccount::leaveRoom(const QString &roomId, const QString &channelType)
{
    if (channelType == QStringLiteral("c")) {
        restApi()->leaveChannel(roomId);
    } else if (channelType == QStringLiteral("p")) {
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
        connect(mDdp, &DDPClient::loginStatusChanged, this, &RocketChatAccount::loginStatusChanged);
        connect(mDdp, &DDPClient::connectedChanged, this, &RocketChatAccount::connectedChanged);
        connect(mDdp, &DDPClient::changed, this, &RocketChatAccount::changed);
        connect(mDdp, &DDPClient::added, this, &RocketChatAccount::added);
        connect(mDdp, &DDPClient::removed, this, &RocketChatAccount::removed);

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

DDPClient::LoginStatus RocketChatAccount::loginStatus()
{
    if (mDdp) {
        return ddp()->loginStatus();
    } else {
        return DDPClient::LoggedOut;
    }
}

void RocketChatAccount::tryLogin()
{
    qCDebug(RUQOLA_LOG) << "Attempting login" << mSettings->userName() << "on" << mSettings->serverUrl();

    delete mDdp;
    mDdp = nullptr;

    // This creates a new ddp() object.
    // DDP will automatically try to connect and login.
    ddp();

    // In the meantime, load cache...
    mRoomModel->reset();
}

void RocketChatAccount::logOut()
{
    mSettings->logout();

    mRoomModel->clear();
#ifdef USE_REASTAPI_JOB
    restApi()->logout();
#else
    QJsonObject user;
    user[QStringLiteral("username")] = mSettings->userName();
    QJsonObject json;
    json[QStringLiteral("user")] = user;
    ddp()->method(QStringLiteral("logout"), QJsonDocument(json));
#endif
    delete mDdp;
    mDdp = nullptr;
    Q_EMIT logoutDone(accountName());
    Q_EMIT loginStatusChanged();
    qCDebug(RUQOLA_LOG) << "Successfully logged out!";
}

void RocketChatAccount::clearUnreadMessages(const QString &roomId)
{
    restApi()->markAsRead(roomId);
}

void RocketChatAccount::changeFavorite(const QString &roomId, bool checked)
{
    if (mRuqolaServerConfig->hasAtLeastVersion(0, 64, 0)) {
        restApi()->markAsFavorite(roomId, checked);
    } else {
        ddp()->toggleFavorite(roomId, checked);
    }
}

void RocketChatAccount::openChannel(const QString &url)
{
    //qCDebug(RUQOLA_LOG) << " void RocketChatAccount::openChannel(const QString &url)"<<url;
    restApi()->channelJoin(url, QString());
    //TODO search correct room + select it.
}

void RocketChatAccount::setChannelJoinDone(const QString &roomId)
{
    ddp()->subscribeRoomMessage(roomId);
}

void RocketChatAccount::openArchivedRoom(const QString &roomId)
{
    //TODO
}

void RocketChatAccount::joinJitsiConfCall(const QString &roomId)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::joinJitsiConfCall(const QString &roomId)"<<roomId;
    const QString hash = QString::fromLatin1(QCryptographicHash::hash((mRuqolaServerConfig->uniqueId() + roomId).toUtf8(), QCryptographicHash::Md5).toHex());
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    const QString scheme = "org.jitsi.meet://";
#else
    const QString scheme = QStringLiteral("https://");
#endif
    const QString url = scheme + mRuqolaServerConfig->jitsiMeetUrl() + QLatin1Char('/') + mRuqolaServerConfig->jitsiMeetPrefix() + hash;
    const QUrl clickedUrl = QUrl::fromUserInput(url);
    QDesktopServices::openUrl(clickedUrl);
}

void RocketChatAccount::eraseRoom(const QString &roomId, const QString &channelType)
{
    if (channelType == QStringLiteral("c")) {
        restApi()->channelDelete(roomId);
    } else if (channelType == QStringLiteral("p")) {
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
    ddp()->openDirectChannel(username);
//#endif
}

void RocketChatAccount::createNewChannel(const QString &name, bool readOnly, bool privateRoom, const QString &userNames, bool encryptedRoom, const QString &password, bool broadcast)
{
    //TODO use encryted room
    //TODO use broadcast
    if (!name.trimmed().isEmpty()) {
        const QStringList lstUsers = userNames.split(QLatin1Char(','), QString::SkipEmptyParts);
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

void RocketChatAccount::joinRoom(const QString &roomId, const QString &joinCode)
{
    restApi()->channelJoin(roomId, joinCode);
}

void RocketChatAccount::channelAndPrivateAutocomplete(const QString &pattern)
{
    //TODO use restapi
    if (pattern.isEmpty()) {
        searchChannelModel()->clear();
    } else {
        //Use restapi
        //Avoid to show own user
#ifdef USE_REASTAPI_JOB
        restApi()->searchRoomUser(pattern);
#else
        const QString addUserNameToException = userName();
        ddp()->channelAndPrivateAutocomplete(pattern, addUserNameToException);
#endif
    }
}

void RocketChatAccount::listEmojiCustom()
{
    if (mRuqolaServerConfig->hasAtLeastVersion(0, 63, 0)) {
        restApi()->listEmojiCustom();
    } else {
        ddp()->listEmojiCustom();
    }
}

void RocketChatAccount::setDefaultStatus(User::PresenceStatus status)
{
    //Not implemented yet
    //TODO use restapi
    if (statusModel()->currentUserStatus() != status) {
        ddp()->setDefaultStatus(status);
    }
}

void RocketChatAccount::changeDefaultStatus(int index)
{
    setDefaultStatus(mStatusModel->status(index));
}

void RocketChatAccount::loadEmojiRestApi(const QJsonObject &obj)
{
    mEmojiManager->loadCustomEmoji(obj, true);
}

void RocketChatAccount::loadEmoji(const QJsonObject &obj)
{
    mEmojiManager->loadCustomEmoji(obj, false);
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
    //Clear before to create new search
    userCompleterModel()->clear();
    rocketChatBackend()->clearUsersList();
    if (!searchText.isEmpty()) {
        //Avoid to show own user
        QString addUserNameToException;
        if (exception.isEmpty()) {
            addUserNameToException = userName();
        } else {
            addUserNameToException = exception + QLatin1Char(',') + userName();
        }
        //TODO use restapi
        ddp()->userAutocomplete(searchText, addUserNameToException);
    }
}

void RocketChatAccount::membersInRoom(const QString &roomId, const QString &roomType)
{
    restApi()->membersInRoom(roomId, roomType);
}

void RocketChatAccount::parseUsersForRooms(const QJsonObject &obj, const QString &roomId)
{
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    if (usersModelForRoom) {
        usersModelForRoom->parseUsersForRooms(obj, mUserModel, true);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << roomId;
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

MentionsFilterProxyModel *RocketChatAccount::mentionsFilterProxyModel() const
{
    return mMentionsFilterProxyModel;
}

MentionsModel *RocketChatAccount::mentionsModel() const
{
    return mMentionsModel;
}

ThreadsModel *RocketChatAccount::threadsModel() const
{
    return mThreadsModel;
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

void RocketChatAccount::setEmoticonModel(EmoticonModel *emoticonModel)
{
    mEmoticonModel = emoticonModel;
}

ReceiveTypingNotificationManager *RocketChatAccount::receiveTypingNotificationManager() const
{
    return mReceiveTypingNotificationManager;
}

void RocketChatAccount::slotChannelRolesDone(const QJsonObject &obj, const QString &roomId)
{
    Room *room = mRoomModel->findRoom(roomId);
    if (room) {
        Roles r;
        r.parseRole(obj);
        room->setRolesForRooms(r);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << roomId;
    }
}

void RocketChatAccount::slotGetThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId)
{
    ThreadMessages threadmessages;
    threadmessages.parseThreadMessages(obj);
    //Create own model ??
    mThreadMessageModel->clear();
    for (int i = 0; i < threadmessages.count(); ++i) {
        mThreadMessageModel->addMessage(threadmessages.at(i));
    }
    //USe a specific method for threadmessages
}

void RocketChatAccount::slotGetDiscussionsListDone(const QJsonObject &obj, const QString &roomId)
{
    if (mDiscussionsModel->roomId() != roomId) {
        mDiscussionsModel->parseDiscussions(obj, roomId);
    } else {
        mDiscussionsModel->addMoreDiscussions(obj);
    }
}

void RocketChatAccount::slotGetAllUserMentionsDone(const QJsonObject &obj, const QString &roomId)
{
    if (mMentionsModel->roomId() != roomId) {
        mMentionsModel->parseMentions(obj, roomId);
    } else {
        mMentionsModel->addMoreMentions(obj);
    }
}

void RocketChatAccount::slotGetThreadsListDone(const QJsonObject &obj, const QString &roomId)
{
    if (mThreadsModel->roomId() != roomId) {
        mThreadsModel->parseThreads(obj, roomId);
    } else {
        mThreadsModel->addMoreThreads(obj);
    }
}

void RocketChatAccount::slotSplotLightDone(const QJsonObject &obj)
{
    //qDebug() << " void RocketChatAccount::slotSplotLightDone(const QJsonObject &obj)"<<obj;
    //If empty ! show empty list
    loadAutoCompleteChannel(obj);
}

void RocketChatAccount::slotChannelFilesDone(const QJsonObject &obj, const QString &roomId)
{
    //qDebug() << " slotChannelFilesDone(const QJsonObject &obj, const QString &roomId)" << roomId << " obj " << obj;
    if (mFilesModelForRoom->roomId() != roomId) {
        mFilesModelForRoom->parseFileAttachments(obj, roomId);
    } else {
        mFilesModelForRoom->addMoreFileAttachments(obj);
    }
}

void RocketChatAccount::loadMoreFileAttachments(const QString &roomId, const QString &channelType)
{
    const int offset = mFilesModelForRoom->fileAttachments()->filesCount();
    if (offset < mFilesModelForRoom->fileAttachments()->total()) {
        restApi()->filesInRoom(roomId, channelType, offset, qMin(50, mFilesModelForRoom->fileAttachments()->total() - offset));
    }
}

void RocketChatAccount::loadMoreDiscussions(const QString &roomId)
{
    const int offset = mDiscussionsModel->discussions()->discussionsCount();
    if (offset < mDiscussionsModel->discussions()->total()) {
        restApi()->getDiscussions(roomId, offset, qMin(50, mDiscussionsModel->discussions()->total() - offset));
    }
}

void RocketChatAccount::loadMoreThreads(const QString &roomId)
{
    const int offset = mThreadsModel->threads()->threadsCount();
    if (offset < mThreadsModel->threads()->total()) {
        restApi()->getThreadsList(roomId, offset, qMin(50, mThreadsModel->threads()->total() - offset));
    }
}

void RocketChatAccount::loadThreadMessagesHistory(const QString &roomId, const QString &channelType)
{
    //TODO
}

void RocketChatAccount::loadMoreMentions(const QString &roomId)
{
    const int offset = mMentionsModel->mentions()->mentionsCount();
    if (offset < mMentionsModel->mentions()->total()) {
        restApi()->channelGetAllUserMentions(roomId, offset, qMin(50, mMentionsModel->mentions()->total() - offset));
    }
}

void RocketChatAccount::loadMoreHistorySearch(const QString &roomId)
{
//    const int offset = mMentionsModel->mentions()->mentionsCount();
//    if (offset < mMentionsModel->mentions()->total()) {
//        restApi()->channelGetAllUserMentions(roomId, offset, qMin(50, mMentionsModel->mentions()->total() - offset));
//    }
}

void RocketChatAccount::createJitsiConfCall(const QString &roomId)
{
    //TODO use restapi
    ddp()->createJitsiConfCall(roomId);
    joinJitsiConfCall(roomId);
}

void RocketChatAccount::addUserToRoom(const QString &userId, const QString &roomId, const QString &channelType)
{
    if (channelType == QStringLiteral("c")) {
        restApi()->addUserInChannel(roomId, userId);
    } else if (channelType == QStringLiteral("p")) {
        restApi()->addUserInGroup(roomId, userId);
    }
}

void RocketChatAccount::clearSearchModel()
{
    mSearchMessageModel->clear();
}

void RocketChatAccount::messageSearch(const QString &pattern, const QString &rid)
{
    if (pattern.isEmpty()) {
        clearSearchModel();
    } else {
        restApi()->searchMessages(rid, pattern);
    }
}

void RocketChatAccount::slotSearchMessages(const QJsonObject &obj)
{
    mSearchMessageModel->parseResult(obj, true);
}

void RocketChatAccount::starMessage(const QString &messageId, bool starred)
{
    restApi()->starMessage(messageId, starred);
}

void RocketChatAccount::pinMessage(const QString &messageId, bool pinned)
{
    restApi()->pinMessage(messageId, pinned);
}

void RocketChatAccount::uploadFile(const QString &roomId, const QString &description, const QString &messageText, const QUrl &fileUrl)
{
    restApi()->uploadFile(roomId, description, messageText, fileUrl);
}

void RocketChatAccount::changeChannelSettings(const QString &roomId, RocketChatAccount::RoomInfoType infoType, const QVariant &newValue, const QString &channelType)
{
    switch (infoType) {
    case Announcement:
        if (channelType == QStringLiteral("c")) {
            restApi()->changeChannelAnnouncement(roomId, newValue.toString());
        } else if (channelType == QStringLiteral("p")) {
            //FOR the moment we can't change group announcement with restapi
            if (mRuqolaServerConfig->hasAtLeastVersion(0, 70, 0)) {
                restApi()->changeGroupsAnnouncement(roomId, newValue.toString());
            } else {
                ddp()->setRoomAnnouncement(roomId, newValue.toString());
            }
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change announcement for type " << channelType;
        }
        break;
    case Description:
        if (channelType == QStringLiteral("c")) {
            restApi()->changeChannelDescription(roomId, newValue.toString());
        } else if (channelType == QStringLiteral("p")) {
            restApi()->changeGroupsDescription(roomId, newValue.toString());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change description for type " << channelType;
        }
        break;
    case Name:
        if (channelType == QStringLiteral("c")) {
            restApi()->changeChannelName(roomId, newValue.toString());
        } else if (channelType == QStringLiteral("p")) {
            restApi()->changeGroupName(roomId, newValue.toString());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change name for type " << channelType;
        }
        break;
    case Topic:
        if (channelType == QStringLiteral("c")) {
            restApi()->changeChannelTopic(roomId, newValue.toString());
        } else if (channelType == QStringLiteral("p")) {
            restApi()->changeGroupsTopic(roomId, newValue.toString());
        } else {
            //TODO : change topic in direct channel
            qCWarning(RUQOLA_LOG) << " unsupport change topic for type " << channelType;
        }
        break;
    case ReadOnly:
        if (channelType == QStringLiteral("c")) {
            restApi()->changeChannelReadOnly(roomId, newValue.toBool());
        } else if (channelType == QStringLiteral("p")) {
            restApi()->changeGroupsReadOnly(roomId, newValue.toBool());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change readonly for type " << channelType;
        }
        break;
    case Archive:
        if (channelType == QStringLiteral("c")) {
            restApi()->archiveChannel(roomId, newValue.toBool());
        } else if (channelType == QStringLiteral("p")) {
            restApi()->archiveGroups(roomId, newValue.toBool());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport archiving for type " << channelType;
        }
        break;
    case RoomType:
        if (channelType == QStringLiteral("c")) {
            restApi()->setChannelType(roomId, newValue.toBool());
        } else if (channelType == QStringLiteral("p")) {
            restApi()->setGroupType(roomId, newValue.toBool());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport roomtype for type " << channelType;
        }
        break;
    case Encrypted:
        if (channelType == QStringLiteral("c")) {
            restApi()->changeChannelEncrypted(roomId, newValue.toBool());
        } else if (channelType == QStringLiteral("p")) {
            restApi()->changeGroupsEncrypted(roomId, newValue.toBool());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport encrypted mode for type " << channelType;
        }
        break;
    }
}

void RocketChatAccount::reportMessage(const QString &messageId, const QString &message)
{
    if (mRuqolaServerConfig->hasAtLeastVersion(0, 64, 0)) {
        restApi()->reportMessage(messageId, message);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::reportMessage is not supported before server 0.64";
    }
}

void RocketChatAccount::getThreadMessages(const QString &threadMessageId)
{
    if (mRuqolaServerConfig->hasAtLeastVersion(1, 0, 0)) {
        restApi()->getThreadMessages(threadMessageId);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::getThreadMessages is not supported before server 1.0.0";
    }
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
    QJsonArray configs = obj.value(QLatin1String("result")).toArray();

    for (QJsonValueRef currentConfig : configs) {
        QJsonObject currentConfObject = currentConfig.toObject();
        const QString id = currentConfObject[QStringLiteral("_id")].toString();
        const QVariant value = currentConfObject[QStringLiteral("value")].toVariant();

        if (id == QLatin1String("uniqueID")) {
            mRuqolaServerConfig->setUniqueId(value.toString());
        } else if (id == QLatin1String("Jitsi_Enabled")) {
            mRuqolaServerConfig->setJitsiEnabled(value.toBool());
        } else if (id == QLatin1String("Jitsi_Domain")) {
            mRuqolaServerConfig->setJitsiMeetUrl(value.toString());
        } else if (id == QLatin1String("Jitsi_URL_Room_Prefix")) {
            mRuqolaServerConfig->setJitsiMeetPrefix(value.toString());
        } else if (id == QLatin1String("FileUpload_Storage_Type")) {
            mRuqolaServerConfig->setFileUploadStorageType(value.toString());
        } else if (id == QLatin1String("Message_AllowEditing")) {
            mRuqolaServerConfig->setAllowMessageEditing(value.toBool());
        } else if (id == QLatin1String("Message_AllowEditing_BlockEditInMinutes")) {
            mRuqolaServerConfig->setBlockEditingMessageInMinutes(value.toInt());
        } else if (id == QLatin1String("OTR_Enable")) {
            mRuqolaServerConfig->setOtrEnabled(value.toBool());
        } else if (id.contains(QRegularExpression(QStringLiteral("^Accounts_OAuth_\\w+")))) {
            if (value.toBool()) {
                mRuqolaServerConfig->addOauthService(id);
            }
        } else if (id == QLatin1String("Site_Url")) {
            mRuqolaServerConfig->setSiteUrl(value.toString());
        } else if (id == QLatin1String("Site_Name")) {
            mRuqolaServerConfig->setSiteName(value.toString());
        } else if (id == QLatin1String("E2E_Enable")) {
            mRuqolaServerConfig->setEncryptionEnabled(value.toBool());
        } else if (id == QLatin1String("Message_AllowPinning")) {
            mRuqolaServerConfig->setAllowMessagePinningEnabled(value.toBool());
        } else if (id == QLatin1String("Message_AllowSnippeting")) {
            mRuqolaServerConfig->setAllowMessageSnippetingEnabled(value.toBool());
        } else if (id == QLatin1String("Message_AllowStarring")) {
            mRuqolaServerConfig->setAllowMessageStarringEnabled(value.toBool());
        } else if (id == QLatin1String("Message_AllowDeleting")) {
            mRuqolaServerConfig->setAllowMessageDeletingEnabled(value.toBool());
        } else if (id == QLatin1String("Threads_enabled")) {
            mRuqolaServerConfig->setThreadsEnabled(value.toBool());
        } else if (id == QLatin1String("Discussion_enabled")) {
            mRuqolaServerConfig->setDiscussionEnabled(value.toBool());
        } else {
            qCDebug(RUQOLA_LOG) << "Other public settings id " << id << value;
        }
    }
    fillOauthModel();
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

InputCompleterModel *RocketChatAccount::inputCompleterModel() const
{
    return mInputTextManager->inputCompleterModel();
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
    ManagerDataPaths::self()->initializeAccountPath(accountname);
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

QString RocketChatAccount::code() const
{
    return settings()->code();
}

void RocketChatAccount::setAuthToken(const QString &token)
{
    settings()->setAuthToken(token);
}

void RocketChatAccount::setPassword(const QString &password)
{
    settings()->setPassword(password);
}

void RocketChatAccount::setCode(const QString &code)
{
    settings()->setCode(code);
}

void RocketChatAccount::setUserName(const QString &username)
{
    settings()->setUserName(username);
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
    return mRuqolaServerConfig->allowMessagePinningEnabled();
}

bool RocketChatAccount::allowMessageSnippetingEnabled() const
{
    return mRuqolaServerConfig->allowMessageSnippetingEnabled();
}

bool RocketChatAccount::allowMessageStarringEnabled() const
{
    return mRuqolaServerConfig->allowMessageStarringEnabled();
}

bool RocketChatAccount::allowMessageDeletingEnabled() const
{
    return mRuqolaServerConfig->allowMessageDeletingEnabled();
}

bool RocketChatAccount::threadsEnabled() const
{
    return mRuqolaServerConfig->threadsEnabled();
}

bool RocketChatAccount::discussionEnabled() const
{
    return mRuqolaServerConfig->discussionEnabled();
}

QString RocketChatAccount::serverVersionStr() const
{
    return mRuqolaServerConfig->serverVersionStr();
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
    if (channelType == QStringLiteral("c")) {
        restApi()->channelKick(roomId, userId);
    } else if (channelType == QStringLiteral("p")) {
        restApi()->groupKick(roomId, userId);
    } else {
        qCWarning(RUQOLA_LOG) << " unsupport kickUser room for type " << channelType;
    }
}

void RocketChatAccount::rolesInRoom(const QString &roomId, const QString &channelType)
{
    if (channelType == QStringLiteral("c")) {
        restApi()->getChannelRoles(roomId);
    } else if (channelType == QStringLiteral("p")) {
        restApi()->getGroupRoles(roomId);
    } else if (channelType == QStringLiteral("d")) {
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
    if (channelType == QStringLiteral("c")) {
        switch (roleType) {
        case RocketChatAccount::AddOwner:
            restApi()->channelAddOwner(roomId, userId);
            break;
        case RocketChatAccount::AddLeader:
            if (mRuqolaServerConfig->hasAtLeastVersion(0, 75, 0)) {
                restApi()->channelAddLeader(roomId, userId);
            } else {
                qCWarning(RUQOLA_LOG) << " RocketChatAccount::AddLeader is not supported before server 0.75";
            }
            break;
        case RocketChatAccount::AddModerator:
            restApi()->channelAddModerator(roomId, userId);
            break;
        case RocketChatAccount::RemoveOwner:
            restApi()->channelRemoveOwner(roomId, userId);
            break;
        case RocketChatAccount::RemoveLeader:
            if (mRuqolaServerConfig->hasAtLeastVersion(0, 75, 0)) {
                restApi()->channelRemoveLeader(roomId, userId);
            } else {
                qCWarning(RUQOLA_LOG) << " RocketChatAccount::RemoveLeader is not supported before server 0.75";
            }
            break;
        case RocketChatAccount::RemoveModerator:
            restApi()->channelRemoveModerator(roomId, userId);
            break;
        }
    } else if (channelType == QStringLiteral("p")) {
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

void RocketChatAccount::parseOtr(const QJsonArray &contents)
{
    const Otr t = mOtrManager->parseOtr(contents);
    qDebug() << " void RocketChatAccount::parseOtr(const QJsonArray &contents)"<<t;
    //TODO add notification ?
}

void RocketChatAccount::sendNotification(const QJsonArray &contents)
{
    QString message;
    QString title;
    QString sender;
    Utils::parseNotification(contents, message, title, sender);

    const QString iconFileName = mCache->avatarUrlFromCacheOnly(sender);
    //qDebug() << " iconFileName"<<iconFileName << " sender " << sender;
    QPixmap pix;
    if (!iconFileName.isEmpty()) {
        const QUrl url = QUrl::fromLocalFile(iconFileName);
        //qDebug() << "url.toLocalFile()"<<url.toLocalFile();
        const bool loaded = pix.load(url.toLocalFile().remove(QStringLiteral("file://")), "JPEG");
        //qDebug() << " load pixmap : "<< loaded;
        //qDebug() << " pix " << pix.isNull();
        Q_UNUSED(loaded);
    }
    Q_EMIT notification(title, message, pix);
}

void RocketChatAccount::inputChannelAutocomplete(const QString &pattern, const QString &exceptions)
{
    ddp()->inputChannelAutocomplete(pattern, exceptions);
}

void RocketChatAccount::inputUserAutocomplete(const QString &pattern, const QString &exceptions)
{
    ddp()->inputUserAutocomplete(pattern, exceptions);
}

void RocketChatAccount::inputTextCompleter(const QJsonObject &obj)
{
    mInputTextManager->inputTextCompleter(obj);
}

void RocketChatAccount::displaySearchedMessage(const QJsonObject &obj)
{
    mSearchMessageModel->parseResult(obj);
}

void RocketChatAccount::updateUser(const QJsonObject &object)
{
    mUserModel->updateUser(object);
}

void RocketChatAccount::userStatusChanged(const User &user)
{
    //qDebug() << " void RocketChatAccount::userStatusChanged(const User &user)"<<user.userId() << " userId" << userID();
    if (user.userId() == userID()) {
        //qDebug() << " void RocketChatAccount::userStatusChanged(const User &user) current user !!!!!!!!!!!!" << user;
        statusModel()->setCurrentPresenceStatus(Utils::presenceStatusFromString(user.status()));
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

void RocketChatAccount::channelGetAllUserMentions(const QString &roomId)
{
    mMentionsModel->initialize();
    restApi()->channelGetAllUserMentions(roomId);
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

void RocketChatAccount::createDiscussion(const QString &parentRoomId, const QString &discussionName, const QString &replyMessage, const QString &messageId)
{
    restApi()->createDiscussion(parentRoomId, discussionName, replyMessage, messageId);
}

void RocketChatAccount::threadsInRoom(const QString &roomId)
{
    if (mRuqolaServerConfig->hasAtLeastVersion(1, 0, 0)) {
        mThreadsModel->initialize();
        restApi()->getThreadsList(roomId);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::threadsInRoom is not supported before server 1.0.0";
    }
}

void RocketChatAccount::discussionsInRoom(const QString &roomId)
{
    if (mRuqolaServerConfig->hasAtLeastVersion(1, 0, 0)) {
        restApi()->getDiscussions(roomId);
    } else {
        qCWarning(RUQOLA_LOG) << " RocketChatAccount::discussionsInRoom is not supported before server 1.0.0";
    }
}

void RocketChatAccount::followMessage(const QString &messageId, bool follow)
{
    if (follow) {
        restApi()->followMessage(messageId);
    } else {
        restApi()->unFollowMessage(messageId);
    }
}
