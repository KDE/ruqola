/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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
#include "usersforroommodel.h"
#include "roomfilterproxymodel.h"
#include "ruqolalogger.h"
#include "ruqolaserverconfig.h"

#include <KLocalizedString>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>

#include <ddpapi/ddpclient.h>

#include <restapi/restapirequest.h>

RocketChatAccount::RocketChatAccount(QObject *parent)
    : QObject(parent)
{
    if (!qEnvironmentVariableIsEmpty("RUQOLA_LOGFILE")) {
        mRuqolaLogger = new RuqolaLogger;
    }
    mRuqolaServerConfig = new RuqolaServerConfig;
    mSettings = new RocketChatAccountSettings(this);
    connect(mSettings, &RocketChatAccountSettings::loginStatusChanged, this, &RocketChatAccount::loginStatusChanged);
    connect(mSettings, &RocketChatAccountSettings::serverURLChanged, this, &RocketChatAccount::serverUrlChanged);
    connect(mSettings, &RocketChatAccountSettings::userIDChanged, this, &RocketChatAccount::userIDChanged);
    connect(mSettings, &RocketChatAccountSettings::userNameChanged, this, &RocketChatAccount::userNameChanged);

    mRocketChatBackend = new RocketChatBackend(this, this);
    connect(mRocketChatBackend, &RocketChatBackend::notification, this, &RocketChatAccount::notification);
    mRoomFilterProxyModel = new RoomFilterProxyModel(this);
    mRoomModel = new RoomModel(this, this);
    mRoomFilterProxyModel->setSourceModel(mRoomModel);
    mUserModel = new UsersModel(this);
    mMessageQueue = new MessageQueue(this);
    mTypingNotification = new TypingNotification(this);
    connect(mTypingNotification, &TypingNotification::informTypingStatus, this, &RocketChatAccount::slotInformTypingStatus);
    loadSettings();
    QTimer::singleShot(0, this, &RocketChatAccount::clearModels);
}

RocketChatAccount::~RocketChatAccount()
{
    delete mRuqolaServerConfig;
    delete mRuqolaLogger;
}

void RocketChatAccount::clearModels()
{
    // Clear rooms data and refill it with data in the cache, if there is
    mRoomModel->reset();

    mMessageQueue->loadCache();
    //Try to send queue message
    mMessageQueue->processQueue();
}

RuqolaServerConfig *RocketChatAccount::getRuqolaServerConfig() const
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

UsersForRoomModel *RocketChatAccount::getUsersForRoomModel(const QString &roomId)
{
    UsersForRoomModel *model = nullptr;
    if ((model = mUsersForRoomModels.value(roomId))) {
        return model;
    } else {
        model = new UsersForRoomModel(this);
        model->setCurrentRoomId(roomId);
        mUsersForRoomModels[roomId] = model;
        return model;
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

void RocketChatAccount::attachmentButtonClicked(const QString &roomId)
{
    const QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                          i18n("Select one or more files to open"),
                                                          QDir::homePath(),
                                                          QStringLiteral("%1 (*.png *.jpeg *.jpg)").arg(i18n("Images")));
    if (fileName.isEmpty()) {
        return;
    }

    qCDebug(RUQOLA_LOG) << "Selected Image " << fileName;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qCDebug(RUQOLA_LOG) << "Cannot open the selected file" << fileName;
        return;
    }
    const QString message = QString::fromLatin1(file.readAll().toBase64());
    const QString roomID(roomId);
    const QString type(QStringLiteral("image"));
    sendMessage(roomID, message, type);
}

void RocketChatAccount::sendMessage(const QString &roomID, const QString &message, const QString &type)
{
    QJsonObject json;
    json[QStringLiteral("rid")] = roomID;
    json[QStringLiteral("msg")] = message;
    json[QStringLiteral("type")] = type;

    ddp()->method(QStringLiteral("sendMessage"), QJsonDocument(json), DDPClient::Persistent);
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
    //TODO
}

void RocketChatAccount::joinJitsiConfCall(const QString &roomId)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::joinJitsiConfCall(const QString &roomId)"<<roomId;
    const QString hash = QString::fromLatin1(QCryptographicHash::hash( ( mRuqolaServerConfig->uniqueId() + roomId ).toUtf8(), QCryptographicHash::Md5 ).toHex());
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    const QString scheme = "org.jitsi.meet://";
#else
    const QString scheme = QStringLiteral("https://");
#endif
    const QString url = scheme + mRuqolaServerConfig->jitsiMeetUrl() + QLatin1Char('/') + mRuqolaServerConfig->jitsiMeetPrefix() + hash;
    qDebug() << url;
    const QUrl clickedUrl = QUrl::fromUserInput(url);
    QDesktopServices::openUrl(clickedUrl);
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

void RocketChatAccount::joinRoom(const QString &roomId)
{
    //TODO define joinCode
    ddp()->joinRoom(roomId, QString());
    ddp()->subscribeRoomMessage(roomId);
}

void RocketChatAccount::listEmojiCustom()
{
    ddp()->listEmojiCustom();
}

void RocketChatAccount::setDefaultStatus(User::PresenceStatus status)
{
    ddp()->setDefaultStatus(status);
}

void RocketChatAccount::loadEmoji()
{
    mEmojiList.clear();
    //TODO
}

void RocketChatAccount::createJitsiConfCall(const QString &roomId)
{
    ddp()->createJitsiConfCall(roomId);
    joinJitsiConfCall(roomId);
}

void RocketChatAccount::parsePublicSettings(const QJsonObject &obj)
{
    QJsonArray configs = obj.value(QStringLiteral("result")).toArray();

    for ( const QJsonValueRef &currentConfig : configs ) {
        QJsonObject currentConfObject = currentConfig.toObject();
        const QString id    = currentConfObject[QStringLiteral("_id")].toString();
        const QString value = currentConfObject[QStringLiteral("value")].toString();

        if ( id == QLatin1String("uniqueID") ) {
            mRuqolaServerConfig->setUniqueId(value);
        } else if ( id == QLatin1String("Jitsi_Domain") ) {
            mRuqolaServerConfig->setJitsiMeetUrl(value);
        } else if ( id == QLatin1String("Jitsi_URL_Room_Prefix") ) {
            mRuqolaServerConfig->setJitsiMeetPrefix(value);
        } else {
            qCDebug(RUQOLA_LOG) << "Other public settings id " << id;
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
}
