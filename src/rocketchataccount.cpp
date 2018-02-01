/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "messagemodel.h"
#include "rocketchataccount.h"
#include "roommodel.h"
#include "roomwrapper.h"
#include "typingnotification.h"
#include "usersmodel.h"
#include "ruqola_debug.h"
#include "ruqola.h"
#include "messagequeue.h"
#include "rocketchatbackend.h"
#include "roomfilterproxymodel.h"
#include "ruqolalogger.h"
#include "ruqolaserverconfig.h"
#include "usercompletermodel.h"
#include "statusmodel.h"
#include "utils.h"
#include "rocketchatcache.h"
#include "emojimanager.h"
#include "otrmanager.h"
#include "usersmodelforroom.h"

#include "ddpapi/ddpclient.h"
#include "restapi/restapirequest.h"

#include <QDesktopServices>
#include <QFile>
#include <QTimer>

RocketChatAccount::RocketChatAccount(const QString &accountFileName, QObject *parent)
    : QObject(parent)
{
    //create an uniq file for each account
    if (!qEnvironmentVariableIsEmpty("RUQOLA_LOGFILE")) {
        mRuqolaLogger = new RuqolaLogger;
    }

    mRuqolaServerConfig = new RuqolaServerConfig;
    //TODO add account name.
    mSettings = new RocketChatAccountSettings(accountFileName, this);
    connect(mSettings, &RocketChatAccountSettings::loginStatusChanged, this, &RocketChatAccount::loginStatusChanged);
    connect(mSettings, &RocketChatAccountSettings::serverURLChanged, this, &RocketChatAccount::serverUrlChanged);
    connect(mSettings, &RocketChatAccountSettings::userIDChanged, this, &RocketChatAccount::userIDChanged);
    connect(mSettings, &RocketChatAccountSettings::userNameChanged, this, &RocketChatAccount::userNameChanged);

    mRocketChatBackend = new RocketChatBackend(this, this);

    loadSettings();

    //After loadSettings
    mEmojiManager = new EmojiManager(this);
    mEmojiManager->setServerUrl(mSettings->serverUrl());
    mOtrManager = new OtrManager(this);
    mRoomFilterProxyModel = new RoomFilterProxyModel(this);
    mUserCompleterModel = new UserCompleterModel(this);
    mStatusModel = new StatusModel(this);
    mRoomModel = new RoomModel(this);
    connect(mRoomModel, &RoomModel::needToUpdateNotification, this, &RocketChatAccount::slotNeedToUpdateNotification);
    mRoomFilterProxyModel->setSourceModel(mRoomModel);
    mUserModel = new UsersModel(this);
    connect(mUserModel, &UsersModel::userStatusChanged, mRoomModel, &RoomModel::userStatusChanged);
    mMessageQueue = new MessageQueue(this);
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
    qDeleteAll(mMessageModels);

    delete mRuqolaServerConfig;
    delete mRuqolaLogger;
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

UserCompleterModel *RocketChatAccount::userCompleterModel() const
{
    return mUserCompleterModel;
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

UsersModelForRoomFilterProxyModel *RocketChatAccount::usersModelForRoomFilterProxyModel(const QString &roomId) const
{
    return mRoomModel->usersModelForRoomFilterProxyModel(roomId);
}

RocketChatBackend *RocketChatAccount::rocketChatBackend() const
{
    return mRocketChatBackend;
}

void RocketChatAccount::loadSettings()
{
    mSettings->loadSettings();
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

RoomWrapper *RocketChatAccount::getRoom(const QString &roomId)
{
    return mRoomModel->findRoom(roomId);
}

MessageModel *RocketChatAccount::getMessageModelForRoom(const QString &roomID)
{
    if (MessageModel *model = mMessageModels.value(roomID)) {
        return model;
    } else {
        mMessageModels[roomID] = new MessageModel(roomID, this, this);
        return mMessageModels[roomID];
    }
}

QString RocketChatAccount::getUserCurrentMessage(const QString &roomId)
{
    return mUserCurrentMessage.value(roomId);
}

void RocketChatAccount::setUserCurrentMessage(const QString &message, const QString &roomId)
{
    if (mUserCurrentMessage.contains(roomId)) {
        if (!message.trimmed().isEmpty()) {
            mUserCurrentMessage[roomId] = message;
        } else {
            mUserCurrentMessage.remove(roomId);
        }
    } else {
        if (!message.trimmed().isEmpty()) {
            mUserCurrentMessage.insert(roomId, message);
        }
    }
}

void RocketChatAccount::textEditing(const QString &roomId, const QString &str)
{
    mTypingNotification->setText(roomId, str);
}

void RocketChatAccount::sendMessage(const QString &roomID, const QString &message)
{
    QJsonObject json;
    json[QStringLiteral("rid")] = roomID;
    json[QStringLiteral("msg")] = message;

    ddp()->method(QStringLiteral("sendMessage"), QJsonDocument(json), DDPClient::Persistent);
}

void RocketChatAccount::updateMessage(const QString &roomID, const QString &messageId, const QString &message)
{
    QJsonObject json;
    json[QStringLiteral("rid")] = roomID;
    json[QStringLiteral("msg")] = message;
    json[QStringLiteral("_id")] = messageId;

    ddp()->method(QStringLiteral("updateMessage"), QJsonDocument(json), DDPClient::Persistent);
}

QString RocketChatAccount::avatarUrl(const QString &userId)
{
    return mCache->avatarUrl(userId);
}

void RocketChatAccount::insertAvatarUrl(const QString &userId, const QString &url)
{
    mCache->insertAvatarUrl(userId, url);
}

RestApiRequest *RocketChatAccount::restApi()
{
    if (!mRestApi) {
        mRestApi = new RestApiRequest(this);
        mRestApi->setServerUrl(mSettings->serverUrl());
    }
    return mRestApi;
}

void RocketChatAccount::leaveRoom(const QString &roomId)
{
    ddp()->leaveRoom(roomId);
}

void RocketChatAccount::hideRoom(const QString &roomId)
{
    ddp()->hideRoom(roomId);
}

DDPClient *RocketChatAccount::ddp()
{
    if (!mDdp) {
        mDdp = new DDPClient(this, this);
        connect(mDdp, &DDPClient::loginStatusChanged, this, &RocketChatAccount::loginStatusChanged);
        connect(mDdp, &DDPClient::changed, this, &RocketChatAccount::changed);
        connect(mDdp, &DDPClient::added, this, &RocketChatAccount::added);

        mDdp->setServerUrl(mSettings->serverUrl());
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

    // Reset model views
    foreach (const QString &key, mMessageModels.keys()) {
        MessageModel *m = mMessageModels.take(key);
        delete m;
    }
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

    foreach (const QString &key, mMessageModels.keys()) {
        MessageModel *m = mMessageModels.take(key);
        delete m;
    }

    mRoomModel->clear();

    QJsonObject user;
    user[QStringLiteral("username")] = mSettings->userName();
    QJsonObject json;
    json[QStringLiteral("user")] = user;
    ddp()->method(QStringLiteral("logout"), QJsonDocument(json));

    delete mDdp;
    mDdp = nullptr;
    Q_EMIT loginStatusChanged();
    qCDebug(RUQOLA_LOG) << "Successfully logged out!";
}

void RocketChatAccount::clearUnreadMessages(const QString &roomId)
{
    ddp()->clearUnreadMessages(roomId);
}

void RocketChatAccount::changeFavorite(const QString &roomId, bool checked)
{
    ddp()->toggleFavorite(roomId, checked);
}

void RocketChatAccount::openChannel(const QString &url)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::openChannel(const QString &url)"<<url;
    //TODO search correct room + select it.
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

void RocketChatAccount::eraseRoom(const QString &roomId)
{
    ddp()->eraseRoom(roomId);
}

void RocketChatAccount::openDirectChannel(const QString &username)
{
    ddp()->openDirectChannel(username);
}

void RocketChatAccount::createNewChannel(const QString &name, bool readOnly, bool privateRoom, const QString &userNames)
{
    const QStringList lstUsers = userNames.split(QLatin1Char(','), QString::SkipEmptyParts);
    if (privateRoom) {
        ddp()->createPrivateGroup(name, lstUsers);
    } else {
        ddp()->createChannel(name, lstUsers, readOnly);
    }
}

void RocketChatAccount::joinRoom(const QString &roomId, const QString &joinCode)
{
    ddp()->joinRoom(roomId, joinCode);
    ddp()->subscribeRoomMessage(roomId);
}

void RocketChatAccount::channelAndPrivateAutocomplete(const QString &pattern, const QString &exception)
{
    ddp()->channelAndPrivateAutocomplete(pattern, exception);
}

void RocketChatAccount::listEmojiCustom()
{
    ddp()->listEmojiCustom();
}

void RocketChatAccount::setDefaultStatus(User::PresenceStatus status)
{
    ddp()->setDefaultStatus(status);
}

void RocketChatAccount::changeDefaultStatus(int index)
{
    setDefaultStatus(mStatusModel->status(index));
}

void RocketChatAccount::loadEmoji(const QJsonObject &obj)
{
    mEmojiManager->loadEmoji(obj);
}

void RocketChatAccount::deleteMessage(const QString &messageId)
{
    ddp()->deleteMessage(messageId);
}

void RocketChatAccount::userAutocomplete(const QString &searchText, const QString &exception)
{
    ddp()->userAutocomplete(searchText, exception);
}

void RocketChatAccount::getUsersOfRoom(const QString &roomId)
{
    ddp()->getUsersOfRoom(roomId, true);
}

void RocketChatAccount::parseUsersForRooms(const QString &roomId, const QJsonObject &root)
{
    UsersModelForRoom *usersModelForRoom = roomModel()->usersModelForRoom(roomId);
    if (usersModelForRoom) {
        usersModelForRoom->parseUsersForRooms(root);
    } else {
        qCWarning(RUQOLA_LOG) << " Impossible to find room " << roomId;
    }
}
UsersModelForRoom *RocketChatAccount::usersModelForRoom(const QString &roomId) const
{
    return mRoomModel->usersModelForRoom(roomId);
}

void RocketChatAccount::createJitsiConfCall(const QString &roomId)
{
    ddp()->createJitsiConfCall(roomId);
    joinJitsiConfCall(roomId);
}

void RocketChatAccount::starMessage(const QString &messageId, const QString &rid, bool starred)
{
    ddp()->starMessage(messageId, rid, starred);
}

void RocketChatAccount::uploadFile(const QString &description, const QUrl &fileUrl)
{
    qDebug() << " void RocketChatAccount::uploadFile(const QString &description, const QUrl &fileUrl)"<<description << " file " << fileUrl;
    //TODO
}

void RocketChatAccount::changeChannelSettings(const QString &roomId, RocketChatAccount::RoomInfoType infoType, const QVariant &newValue)
{
    switch (infoType) {
    case Announcement:
        ddp()->setRoomAnnouncement(roomId, newValue.toString());
        break;
    case Description:
        ddp()->setRoomDescription(roomId, newValue.toString());
        break;
    case Name:
        ddp()->setRoomName(roomId, newValue.toString());
        break;
    case Topic:
        ddp()->setRoomTopic(roomId, newValue.toString());
        break;
    case ReadOnly:
        ddp()->setRoomIsReadOnly(roomId, newValue.toBool());
        break;
    case Archive:
        //No argument here.
        ddp()->archiveRoom(roomId);
        break;
    }
}

void RocketChatAccount::parsePublicSettings(const QJsonObject &obj)
{
    QJsonArray configs = obj.value(QLatin1String("result")).toArray();

    for (const QJsonValueRef &currentConfig : configs) {
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
        } else {
            qCDebug(RUQOLA_LOG) << "Other public settings id " << id << value;
        }
    }
}

QString RocketChatAccount::authToken() const
{
    return settings()->authToken();
}

QString RocketChatAccount::userName() const
{
    return settings()->userName();
}

void RocketChatAccount::setAccountName(const QString &servername)
{
    settings()->setAccountName(servername);
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
    mCache->downloadFile(downloadFileUrl, localFile);
}

QUrl RocketChatAccount::attachmentUrl(const QString &url)
{
    return mCache->attachmentUrl(url);
}

void RocketChatAccount::loadHistory(const QString &roomID, bool initial)
{
    MessageModel *roomModel = getMessageModelForRoom(roomID);
    if (roomModel) {
        QJsonArray params;
        params.append(QJsonValue(roomID));
        // Load history
        const qint64 endDateTime = roomModel->lastTimestamp();
        if (initial) {
            params.append(QJsonValue(QJsonValue::Null));
        } else {
            const qint64 startDateTime = roomModel->generateNewStartTimeStamp(endDateTime);
            QJsonObject dateObject;
            dateObject[QStringLiteral("$date")] = QJsonValue(startDateTime);

            //qDebug() << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  << QDateTime::fromMSecsSinceEpoch(startDateTime);
            params.append(dateObject);
        }
        params.append(QJsonValue(50)); // Max number of messages to load;
        QJsonObject dateObject;
        //qDebug() << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID;
        dateObject[QStringLiteral("$date")] = QJsonValue(endDateTime);
        params.append(dateObject);
        ddp()->loadHistory(params);
    } else {
        qCWarning(RUQOLA_LOG) << "Room is not found " << roomID;
    }
}

void RocketChatAccount::setServerVersion(const QString &version)
{
    qDebug() << " void RocketChatAccount::setServerVersion(const QString &version)" << version;
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
    }
    Q_EMIT notification(title, message, pix);
}
