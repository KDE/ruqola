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
#include "emojimanager.h"
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
#include "managerdatapaths.h"
#include "authenticationmanager.h"

#include "ddpapi/ddpclient.h"
#include "receivetypingnotificationmanager.h"
#include "restapirequest.h"
#include "serverconfiginfo.h"

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

    //After loadSettings
    mEmojiManager = new EmojiManager(this);
    mEmojiManager->setServerUrl(mSettings->serverUrl());
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

FilesForRoomFilterProxyModel *RocketChatAccount::filesForRoomFilterProxyModel(const QString &roomId) const
{
    return mRoomModel->filesForRoomFilterProxyModel(roomId);
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

void RocketChatAccount::sendMessage(const QString &roomID, const QString &message)
{
#ifdef USE_REASTAPI_JOB
    restApi()->postMessage(roomID, message);
#else
    QJsonObject json;
    json[QStringLiteral("rid")] = roomID;
    json[QStringLiteral("msg")] = message;

    ddp()->method(QStringLiteral("sendMessage"), QJsonDocument(json), DDPClient::Persistent);
#endif
}

void RocketChatAccount::updateMessage(const QString &roomID, const QString &messageId, const QString &message)
{
#ifdef USE_REASTAPI_JOB
    restApi()->updateMessage(roomID, messageId, message);
#else
    QJsonObject json;
    json[QStringLiteral("rid")] = roomID;
    json[QStringLiteral("msg")] = message;
    json[QStringLiteral("_id")] = messageId;

    ddp()->method(QStringLiteral("updateMessage"), QJsonDocument(json), DDPClient::Persistent);
#endif
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
        connect(mRestApi, &RocketChatRestApi::RestApiRequest::openArchivedRoom, this, &RocketChatAccount::openArchivedRoom);
        mRestApi->setServerUrl(mSettings->serverUrl());
        mRestApi->setRestApiLogger(mRuqolaLogger);
    }
    return mRestApi;
}

void RocketChatAccount::leaveRoom(const QString &roomId, const QString &channelType)
{
#ifdef USE_REASTAPI_JOB
    if (channelType == QStringLiteral("c")) {
        restApi()->leaveChannel(roomId);
    } else if (channelType == QStringLiteral("p")) {
        restApi()->leaveGroups(roomId);
    } else {
        qCWarning(RUQOLA_LOG) << " unsupport leave room for type " << channelType;
    }
#else
    Q_UNUSED(channelType)
    ddp()->leaveRoom(roomId);
#endif
}

void RocketChatAccount::hideRoom(const QString &roomId, const QString &channelType)
{
#ifdef USE_REASTAPI_JOB
    restApi()->closeChannel(roomId, channelType);
#else
    Q_UNUSED(channelType)
    ddp()->hideRoom(roomId);
#endif
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
#ifdef USE_REASTAPI_JOB
    restApi()->markAsRead(roomId);
#else
    //TODO don't send message when we don't have unread message
    ddp()->clearUnreadMessages(roomId);
#endif
}

void RocketChatAccount::changeFavorite(const QString &roomId, bool checked)
{
#ifdef USE_REASTAPI_JOB
    if (mRuqolaServerConfig->hasAtLeastVersion(0, 64, 0)) {
        restApi()->markAsFavorite(roomId, checked);
    } else {
        ddp()->toggleFavorite(roomId, checked);
    }
#else
    ddp()->toggleFavorite(roomId, checked);
#endif
}

void RocketChatAccount::openChannel(const QString &url)
{
    //TODO use restapi
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::openChannel(const QString &url)"<<url;
#ifdef USE_REASTAPI_JOB
    restApi()->channelJoin(url, QString());
#else
    ddp()->joinRoom(url, QString());
#endif
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
#ifdef USE_REASTAPI_JOB_NOT_SUPPORTED_YET
    //Leave ! or close for direct channel.
    //restApi()->closeChannel(roomId, channelType);
#else
    Q_UNUSED(channelType);
    ddp()->eraseRoom(roomId);
#endif
}

void RocketChatAccount::openDirectChannel(const QString &username)
{
#ifdef USE_REASTAPI_JOB
    restApi()->createDirectMessage(username);
#else
    ddp()->openDirectChannel(username);
#endif
}

void RocketChatAccount::createNewChannel(const QString &name, bool readOnly, bool privateRoom, const QString &userNames, bool encryptedRoom, const QString &password)
{
    //TODO password
    //TODO use encryted room
    if (!name.trimmed().isEmpty()) {
        const QStringList lstUsers = userNames.split(QLatin1Char(','), QString::SkipEmptyParts);
#ifdef USE_REASTAPI_JOB
        if (privateRoom) {
            restApi()->createGroups(name, readOnly, lstUsers);
        } else {
            restApi()->createChannels(name, readOnly, lstUsers, password);
        }
#else
        if (privateRoom) {
            ddp()->createPrivateGroup(name, lstUsers);
        } else {
            ddp()->createChannel(name, lstUsers, readOnly);
        }
#endif
    } else {
        qCDebug(RUQOLA_LOG) << "Channel name can't be empty";
    }
}

void RocketChatAccount::joinRoom(const QString &roomId, const QString &joinCode)
{
#ifdef USE_REASTAPI_JOB
    restApi()->channelJoin(roomId, joinCode);
#else
    //TODO use restapi
    ddp()->joinRoom(roomId, joinCode);
#endif
}

void RocketChatAccount::channelAndPrivateAutocomplete(const QString &pattern)
{
    //TODO use restapi
    if (pattern.isEmpty()) {
        searchChannelModel()->clear();
    } else {
        //Avoid to show own user
        const QString addUserNameToException = userName();
        ddp()->channelAndPrivateAutocomplete(pattern, addUserNameToException);
    }
}

void RocketChatAccount::listEmojiCustom()
{
#ifdef USE_REASTAPI_JOB
    if (mRuqolaServerConfig->hasAtLeastVersion(0, 63, 0)) {
#ifdef USE_REASTAPI_JOB
        restApi()->listEmojiCustom();
        connect(restApi(), &RocketChatRestApi::RestApiRequest::loadEmojiCustomDone, this, &RocketChatAccount::loadEmojiRestApi, Qt::UniqueConnection);
#else
        ddp()->listEmojiCustom();
#endif
    } else {
        ddp()->listEmojiCustom();
    }
#else
    ddp()->listEmojiCustom();
#endif
}

void RocketChatAccount::setDefaultStatus(User::PresenceStatus status)
{
    //Not implemented yet
    //TODO use restapi
    ddp()->setDefaultStatus(status);
}

void RocketChatAccount::changeDefaultStatus(int index)
{
    setDefaultStatus(mStatusModel->status(index));
}

void RocketChatAccount::loadEmojiRestApi(const QJsonObject &obj)
{
    mEmojiManager->loadEmoji(obj, true);
}

void RocketChatAccount::loadEmoji(const QJsonObject &obj)
{
    mEmojiManager->loadEmoji(obj, false);
}

void RocketChatAccount::deleteMessage(const QString &messageId, const QString &roomId)
{
#ifdef USE_REASTAPI_JOB
    restApi()->deleteMessage(roomId, messageId);
#else
    Q_UNUSED(roomId);
    ddp()->deleteMessage(messageId);
#endif
}

void RocketChatAccount::insertFilesList(const QString &roomId)
{
    FilesForRoomModel *filesForRoomModel = roomModel()->filesModelForRoom(roomId);
    if (filesForRoomModel) {
        filesForRoomModel->setFiles(rocketChatBackend()->files());
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << roomId;
    }
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

void RocketChatAccount::getUsersOfRoom(const QString &roomId)
{
    //TODO use restapi
    ddp()->getUsersOfRoom(roomId, true);
}

void RocketChatAccount::parseUsersForRooms(const QString &roomId, const QJsonObject &root)
{
    UsersForRoomModel *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    if (usersModelForRoom) {
        usersModelForRoom->parseUsersForRooms(root, mUserModel);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << roomId;
    }
}

void RocketChatAccount::loadAutoCompleteChannel(const QJsonObject &obj)
{
    mSearchChannelModel->parseChannels(obj);
}

UsersForRoomModel *RocketChatAccount::usersModelForRoom(const QString &roomId) const
{
    return mRoomModel->usersModelForRoom(roomId);
}

void RocketChatAccount::roomFiles(const QString &roomId, const QString &channelType)
{
#ifdef USE_REASTAPI_JOB
    connect(restApi(), &RocketChatRestApi::RestApiRequest::channelFilesDone, this, &RocketChatAccount::slotChannelFilesDone, Qt::UniqueConnection);
    restApi()->filesInRoom(roomId, channelType);
#else
    Q_UNUSED(channelType);
    rocketChatBackend()->clearFilesList();
    ddp()->roomFiles(roomId);
#endif
}

QVector<File> RocketChatAccount::parseFilesInChannel(const QJsonObject &obj)
{
    //TODO add autotests
    QVector<File> files;
    const QJsonArray filesArray = obj.value(QLatin1String("files")).toArray();
    files.reserve(filesArray.count());
    for (int i = 0; i < filesArray.count(); ++i) {
        QJsonObject fileObj = filesArray.at(i).toObject();
        File f;
        f.parseFile(fileObj, true);
        files.append(f);
    }
    return files;
}

ReceiveTypingNotificationManager *RocketChatAccount::receiveTypingNotificationManager() const
{
    return mReceiveTypingNotificationManager;
}

void RocketChatAccount::slotChannelFilesDone(const QJsonObject &obj, const QString &roomId)
{
    const QVector<File> files = parseFilesInChannel(obj);
    FilesForRoomModel *filesForRoomModel = roomModel()->filesModelForRoom(roomId);
    if (filesForRoomModel) {
        filesForRoomModel->setFiles(files);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << roomId;
    }
}

void RocketChatAccount::createJitsiConfCall(const QString &roomId)
{
    //TODO use restapi
    ddp()->createJitsiConfCall(roomId);
    joinJitsiConfCall(roomId);
}

void RocketChatAccount::addUserToRoom(const QString &username, const QString &roomId, const QString &channelType)
{
#ifdef USE_REASTAPI_JOB_IMPOSSIBLE_YET
    restApi()->addUserInChannel(roomId, username);
    restApi()->addUserInGroup(roomId, username);
#else
    Q_UNUSED(channelType);
    ddp()->addUserToRoom(username, roomId);
#endif
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
#ifdef USE_REASTAPI_JOB
        connect(restApi(), &RocketChatRestApi::RestApiRequest::searchMessageDone, this, &RocketChatAccount::slotSearchMessages);
        restApi()->searchMessages(rid, pattern);
#else
        ddp()->messageSearch(rid, pattern);
#endif
    }
}

void RocketChatAccount::slotSearchMessages(const QJsonObject &obj)
{
    mSearchMessageModel->parseResult(obj, true);
}

void RocketChatAccount::starMessage(const QString &messageId, const QString &rid, bool starred)
{
#ifdef USE_REASTAPI_JOB
    Q_UNUSED(rid);
    restApi()->starMessage(messageId, starred);
#else
    ddp()->starMessage(messageId, rid, starred);
#endif
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
#ifdef USE_REASTAPI_JOB
            restApi()->changeChannelName(roomId, newValue.toString());
#else
            ddp()->setRoomName(roomId, newValue.toString());
#endif
        } else if (channelType == QStringLiteral("p")) {
            restApi()->changeGroupName(roomId, newValue.toString());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change name for type " << channelType;
        }
        break;
    case Topic:
#ifdef USE_REASTAPI_JOB
        if (channelType == QStringLiteral("c")) {
            restApi()->changeChannelTopic(roomId, newValue.toString());
        } else if (channelType == QStringLiteral("p")) {
            restApi()->changeGroupsTopic(roomId, newValue.toString());
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport change topic for type " << channelType;
        }

#else
        ddp()->setRoomTopic(roomId, newValue.toString());
#endif
        break;
    case ReadOnly:
        ddp()->setRoomIsReadOnly(roomId, newValue.toBool());
        break;
    case Archive:
#ifdef USE_REASTAPI_JOB
        if (channelType == QStringLiteral("c")) {
            restApi()->archiveChannel(roomId);
        } else if (channelType == QStringLiteral("p")) {
            restApi()->archiveGroups(roomId);
        } else {
            qCWarning(RUQOLA_LOG) << " unsupport archiving for type " << channelType;
        }
#else
        //No argument here.
        ddp()->archiveRoom(roomId);
#endif
        break;
    case RoomType:
        ddp()->setRoomType(roomId, newValue.toBool());
        break;
    case Encrypted:
        //TODO use restApi when there is some api
        ddp()->setRoomEncrypted(roomId, newValue.toBool());
        break;
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

void RocketChatAccount::setAuthToken(const QString &token)
{
    settings()->setAuthToken(token);
}

void RocketChatAccount::setPassword(const QString &password)
{
    settings()->setPassword(password);
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

QString RocketChatAccount::serverVersionStr() const
{
    return mRuqolaServerConfig->serverVersionStr();
}

ServerConfigInfo *RocketChatAccount::serverConfigInfo() const
{
    return mServerConfigInfo;
}

void RocketChatAccount::groupInfo(const QString &roomId)
{
    restApi()->groupInfo(roomId);
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
    ddp()->ignoreUser(rid, userId, ignore);
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

void RocketChatAccount::initializeRoom(const QString &roomId, bool loadInitialHistory)
{
    ddp()->subscribeRoomMessage(roomId);
    getUsersOfRoom(roomId);

    if (loadInitialHistory) {
        //Load history
        //TODO fix me use channeltype!
        loadHistory(roomId, QString(), true /*initial loading*/);
    }
}

void RocketChatAccount::openDocumentation()
{
    QDesktopServices::openUrl(QUrl(QStringLiteral("help:/")));
}
