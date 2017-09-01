/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ruqola.h"
#include "roommodel.h"
#include "typingnotification.h"
#include "ddpapi/ddpclient.h"
#include "notification.h"
#include "messagequeue.h"
#include "ruqola_debug.h"
#include "rocketchataccount.h"
#include "restapi/restapirequest.h"
#include <QFileDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>

Ruqola::Ruqola(QObject *parent)
    : QObject(parent)
{
    //Todo load all account
    mRocketChatAccount = new RocketChatAccount(this);
    connect(mRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &Ruqola::loginStatusChanged);
    connect(mRocketChatAccount, &RocketChatAccount::serverURLChanged, this, &Ruqola::serverURLChanged);
    connect(mRocketChatAccount, &RocketChatAccount::userIDChanged, this, &Ruqola::userIDChanged);
    connect(mRocketChatAccount, &RocketChatAccount::userNameChanged, this, &Ruqola::userNameChanged);
    connect(mRocketChatAccount, &RocketChatAccount::notification, this, &Ruqola::sendNotification);
}

RocketChatAccount *Ruqola::rocketChatAccount() const
{
    return mRocketChatAccount;
}

Ruqola *Ruqola::self()
{
    static Ruqola *s_self = nullptr;
    if (!s_self) {
        s_self = new Ruqola;
        // Create systray to show notifications on Desktop
#if !defined(Q_OS_ANDROID) || !defined(Q_OS_IOS)
        s_self->notification();
#endif
    }
    return s_self;
}

QString Ruqola::authToken() const
{
    return mRocketChatAccount->settings()->authToken();
}

QString Ruqola::userName() const
{
    return mRocketChatAccount->settings()->userName();
}

QString Ruqola::userID() const
{
    return mRocketChatAccount->settings()->userId();
}

QString Ruqola::password() const
{
    return mRocketChatAccount->settings()->password();
}

void Ruqola::setAuthToken(const QString &token)
{
    mRocketChatAccount->settings()->setAccountName(token);
}

void Ruqola::setPassword(const QString &password)
{
    mRocketChatAccount->settings()->setPassword(password);
}

void Ruqola::setUserName(const QString &username)
{
    mRocketChatAccount->settings()->setUserName(username);
}

void Ruqola::setUserID(const QString &userID)
{
    mRocketChatAccount->settings()->setUserId(userID);
}

RoomModel *Ruqola::roomModel() const
{
    return mRocketChatAccount->roomModel();
}

RestApiRequest *Ruqola::restapi() const
{
    return mRocketChatAccount->restapi();
}

UsersForRoomModel *Ruqola::getUsersForRoomModel(const QString &roomId)
{
    return mRocketChatAccount->getUsersForRoomModel(roomId);
}

UsersModel *Ruqola::usersModel() const
{
    return mRocketChatAccount->usersModel();
}

void Ruqola::sendNotification(const QString &title, const QString &message)
{
    notification()->showMessage(title, message, QSystemTrayIcon::Information, 5000);
}

DDPClient *Ruqola::ddp() const
{
    return mRocketChatAccount->ddp();
}

Notification *Ruqola::notification()
{
    if (!mNotification) {
        mNotification = new Notification();
        mNotification->show();
    }
    return mNotification;
}

void Ruqola::attachmentButtonClicked(const QString &roomId)
{
    mRocketChatAccount->attachmentButtonClicked(roomId);
}

void Ruqola::clearUnreadMessages(const QString &roomId)
{
    mRocketChatAccount->clearUnreadMessages(roomId);
}

QString Ruqola::getUserCurrentMessage(const QString &roomId)
{
    return mRocketChatAccount->getUserCurrentMessage(roomId);
}

void Ruqola::setUserCurrentMessage(const QString &message, const QString &roomId)
{
    mRocketChatAccount->setUserCurrentMessage(message, roomId);
}

void Ruqola::textEditing(const QString &roomId, const QString &str)
{
    mRocketChatAccount->textEditing(roomId, str);
}

void Ruqola::sendMessage(const QString &roomID, const QString &message, const QString &type)
{
    mRocketChatAccount->sendMessage(roomID, message, type);
}

void Ruqola::leaveRoom(const QString &roomID)
{
    mRocketChatAccount->leaveRoom(roomID);
}

void Ruqola::hideRoom(const QString &roomID)
{
    mRocketChatAccount->hideRoom(roomID);
}

MessageModel *Ruqola::getMessageModelForRoom(const QString &roomID)
{
    return mRocketChatAccount->getMessageModelForRoom(roomID);
}

QString Ruqola::serverURL() const
{
    return mRocketChatAccount->settings()->serverUrl();
}

void Ruqola::setServerURL(const QString &serverURL)
{
    mRocketChatAccount->settings()->setServerUrl(serverURL);
}

DDPClient::LoginStatus Ruqola::loginStatus()
{
    return mRocketChatAccount->loginStatus();
}

void Ruqola::tryLogin()
{
    mRocketChatAccount->tryLogin();
}

void Ruqola::logOut()
{
    mRocketChatAccount->logOut();
}

RoomWrapper *Ruqola::getRoom(const QString &roomID)
{
    return mRocketChatAccount->getRoom(roomID);
}

void Ruqola::slotInformTypingStatus(const QString &room, bool typing)
{
    mRocketChatAccount->slotInformTypingStatus(room, typing);
}
