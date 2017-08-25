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
#include "usermodel.h"
#include "ruqola_debug.h"
#include "ruqola.h"
#include "messagequeue.h"

#include <QFile>
#include <QFileDialog>

#include <ddpapi/ddpclient.h>

#include <restapi/restapirequest.h>

RocketChatAccount::RocketChatAccount(QObject *parent)
    : QObject(parent)
{
    mSettings = new RocketChatAccountSettings(this);
    connect(mSettings, &RocketChatAccountSettings::loginStatusChanged, this, &RocketChatAccount::loginStatusChanged);
    connect(mSettings, &RocketChatAccountSettings::serverURLChanged, this, &RocketChatAccount::serverURLChanged);
    connect(mSettings, &RocketChatAccountSettings::userIDChanged, this, &RocketChatAccount::userIDChanged);
    connect(mSettings, &RocketChatAccountSettings::userNameChanged, this, &RocketChatAccount::userNameChanged);


    mRoomModel = new RoomModel(this);
    mUserModel = new UsersModel(this);
    mMessageQueue = new MessageQueue(this);
    mTypingNotification = new TypingNotification(this);
    connect(mTypingNotification, &TypingNotification::informTypingStatus, this, &RocketChatAccount::slotInformTypingStatus);
    loadSettings();
    QTimer::singleShot(0, this, &RocketChatAccount::slotInitializeMessageQueue);
}

RocketChatAccount::~RocketChatAccount()
{

}

void RocketChatAccount::slotInitializeMessageQueue()
{
    mMessageQueue->loadCache();
    //Try to send queue message
    mMessageQueue->processQueue();
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

UsersModel *RocketChatAccount::userModel() const
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
        mMessageModels[roomID] = new MessageModel(roomID, this);
        return mMessageModels[roomID];
    }
}


void RocketChatAccount::textEditing(const QString &roomId, const QString &str)
{
    mTypingNotification->setText(roomId, str);
}

void RocketChatAccount::attachmentButtonClicked(const QString &roomId)
{
    const QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                          tr("Select one or more files to open"),
                                                          QDir::homePath(),
                                                          tr("Images (*.png *.jpeg *.jpg)"));
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

RestApiRequest *RocketChatAccount::restApi() const
{
    return mRestApi;
}

RestApiRequest *RocketChatAccount::restapi()
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
        mDdp->setServerUrl(mSettings->serverUrl());
        mDdp->start();
        connect(mDdp, &DDPClient::loginStatusChanged, this, &RocketChatAccount::loginStatusChanged);
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

    //FIXME
    //TODO we need to load it after ddp login done
    restapi();
    restapi()->setPassword(mSettings->password());
    restapi()->login();

    // In the meantime, load cache...
    //if(Ruqola::self()->ddp()->isConnected() && Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
    mRoomModel->reset();
    //}
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
