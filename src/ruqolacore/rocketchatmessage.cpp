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

#include "rocketchatmessage.h"
#include "ruqola_debug.h"
#include "utils.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QCryptographicHash>

RocketChatMessage::RocketChatMessage()
    : mJsonFormat(QJsonDocument::Compact)
{
}

void RocketChatMessage::setJsonFormat(const QJsonDocument::JsonFormat &jsonFormat)
{
    mJsonFormat = jsonFormat;
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::getRoomRoles(const QString &roomID, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID)
    };
    return generateMethod(QStringLiteral("getRoomRoles"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setReaction(const QString &emoji, const QString &messageId, quint64 id)
{
    const QJsonArray params{
        QJsonValue(emoji),
        QJsonValue(messageId)
    };
    return generateMethod(QStringLiteral("setReaction"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::messageSearch(const QString &roomId, const QString &pattern, quint64 id)
{
    const QJsonArray params{
        QJsonValue(pattern),
        QJsonValue(roomId)
        //TODO limit
    };
    return generateMethod(QStringLiteral("messageSearch"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::openDirectChannel(const QString &username, quint64 id)
{
    const QJsonArray params{
        QJsonValue(username)
    };
    return generateMethod(QStringLiteral("createDirectMessage"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::createChannel(const QString &roomName, const QStringList &userList, bool readOnly, quint64 id)
{
    const QJsonArray params{
        {
            roomName
        }, {
            QJsonArray::fromStringList(userList)
        }, {
            readOnly
        }
    };

    return generateMethod(QStringLiteral("createChannel"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::createPrivateGroup(const QString &roomName, const QStringList &userList, quint64 id)
{
    const QJsonArray params{
        {
            roomName
        },
        {
            QJsonArray::fromStringList(userList)
        }
    };

    return generateMethod(QStringLiteral("createPrivateGroup"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::eraseRoom(const QString &roomID, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID)
    };
    return generateMethod(QStringLiteral("eraseRoom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::archiveRoom(const QString &roomID, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID)
    };
    return generateMethod(QStringLiteral("archiveRoom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::unarchiveRoom(const QString &roomID, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID)
    };
    return generateMethod(QStringLiteral("unarchiveRoom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::readMessages(const QString &roomID, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID)
    };
    return generateMethod(QStringLiteral("readMessages"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::openRoom(const QString &roomID, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID)
    };
    return generateMethod(QStringLiteral("openRoom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::leaveRoom(const QString &roomID, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID)
    };
    return generateMethod(QStringLiteral("leaveRoom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::hideRoom(const QString &roomID, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID)
    };
    return generateMethod(QStringLiteral("hideRoom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::toggleFavorite(const QString &roomID, bool favorite, quint64 id)
{
    const QJsonArray params{
        QJsonValue(roomID), {
            favorite
        }
    };
    return generateMethod(QStringLiteral("toggleFavorite"), QJsonDocument(params), id);
}

//TODO verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::listEmojiCustom(quint64 id)
{
    const QJsonArray params;
    return generateMethod(QStringLiteral("listEmojiCustom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomName(const QString &roomId, const QString &name, quint64 id)
{
    return saveRoomSettings(QStringLiteral("roomName"), roomId, name, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomTopic(const QString &roomId, const QString &topic, quint64 id)
{
    return saveRoomSettings(QStringLiteral("roomTopic"), roomId, topic, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomEncrypted(const QString &roomId, bool encrypted, quint64 id)
{
    return saveRoomSettings(QStringLiteral("encrypted"), roomId, encrypted, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomDescription(const QString &roomId, const QString &description, quint64 id)
{
    return saveRoomSettings(QStringLiteral("roomDescription"), roomId, description, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomAnnouncement(const QString &roomId, const QString &announcement, quint64 id)
{
    return saveRoomSettings(QStringLiteral("roomAnnouncement"), roomId, announcement, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomType(const QString &roomId, bool privateChannel, quint64 id)
{
    return saveRoomSettings(QStringLiteral("roomType"), roomId, privateChannel ? QStringLiteral("p") : QStringLiteral("c"), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomIsReadOnly(const QString &roomId, bool readOnly, quint64 id)
{
    return saveRoomSettings(QStringLiteral("readOnly"), roomId, readOnly, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomHasSystemMessages(const QString &roomId, bool systemMessages, quint64 id)
{
    return saveRoomSettings(QStringLiteral("systemMessages"), roomId, systemMessages, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomIsDefault(const QString &roomId, bool isDefault, quint64 id)
{
    return saveRoomSettings(QStringLiteral("default"), roomId, isDefault, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomJoinCode(const QString &roomId, const QString &joinCode, quint64 id)
{
    return saveRoomSettings(QStringLiteral("joinCode"), roomId, joinCode, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::blockUser(const QString &rid, const QString &userId, quint64 id)
{
    QJsonObject obj{
        {
            QStringLiteral("rid"), rid
        },
        {
            QStringLiteral("blocked"), userId
        },
    };
    const QJsonArray params{{
        obj
    }};

    return generateMethod(QStringLiteral("blockUser"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::ignoreUser(const QString &roomId, const QString &userId, bool ignore, quint64 id)
{
    QJsonObject obj{
        {
            QStringLiteral("userId"), userId
        },
        {
            QStringLiteral("rid"), roomId
        },
        {
            QStringLiteral("ignore"), ignore
        }
    };
    const QJsonArray params{{
        obj
    }};

    return generateMethod(QStringLiteral("ignoreUser"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::unblockUser(const QString &rid, const QString &userId, quint64 id)
{
    QJsonObject obj{
        {
            QStringLiteral("rid"), rid
        },
        {
            QStringLiteral("blocked"), userId
        },
    };
    const QJsonArray params{{
        obj
    }};

    return generateMethod(QStringLiteral("unblockUser"), QJsonDocument(params), id);
}

//TODO verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::saveNotificationsSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id)
{
    const QJsonArray params{{
        roomId
    }, {
            key
        }, {
            value
        }};
    return generateMethod(QStringLiteral("saveNotificationSettings"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::disableNotifications(const QString &roomId, bool disabled, quint64 id)
{
    return saveNotificationsSettings(QStringLiteral("disableNotifications"), roomId, QJsonValue(disabled ? QLatin1String("1") : QLatin1String("0")), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::hideUnreadStatus(const QString &roomId, bool disabled, quint64 id)
{
    return saveNotificationsSettings(QStringLiteral("hideUnreadStatus"), roomId, QJsonValue(disabled ? QLatin1String("1") : QLatin1String("0")), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::audioNotifications(const QString &roomId, const QString &value, quint64 id)
{
    return saveNotificationsSettings(QStringLiteral("audioNotifications"), roomId, value, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::desktopNotifications(const QString &roomId, const QString &value, quint64 id)
{
    return saveNotificationsSettings(QStringLiteral("desktopNotifications"), roomId, value, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::mobilePushNotifications(const QString &roomId, const QString &value, quint64 id)
{
    return saveNotificationsSettings(QStringLiteral("mobilePushNotifications"), roomId, value, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::emailNotifications(const QString &roomId, const QString &value, quint64 id)
{
    return saveNotificationsSettings(QStringLiteral("emailNotifications"), roomId, value, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::unreadAlert(const QString &roomId, const QString &value, quint64 id)
{
    return saveNotificationsSettings(QStringLiteral("unreadAlert"), roomId, value, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::saveRoomSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id)
{
    const QJsonArray params{{
        roomId
    }, {
            key
        }, {
            value
        }};
    return generateMethod(QStringLiteral("saveRoomSettings"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::joinRoom(const QString &roomId, const QString &accessCode, quint64 id)
{
    const QJsonArray params{{
        roomId
    }, {
            accessCode
        }};
    return generateMethod(QStringLiteral("joinRoom"), QJsonDocument(params), id);
}

//Verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::deleteFileMessage(const QString &fileId, quint64 id)
{
    const QJsonArray params{{
        fileId
    }};
    return generateMethod(QStringLiteral("deleteFileMessage"), QJsonDocument(params), id);
}

//Verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::updateMessage(quint64 id)
{
    const QJsonArray params;
    return generateMethod(QStringLiteral("updateMessage"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::deleteMessage(const QString &messageId, quint64 id)
{
    QJsonObject obj{{
        QStringLiteral("_id"), messageId
    }};
    const QJsonArray params{{
        obj
    }};
    return generateMethod(QStringLiteral("deleteMessage"), QJsonDocument(params), id);
}

//TODO verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::starMessage(const QString &_id, const QString &rid, bool starred, quint64 id)
{
    QJsonObject obj{
        {
            QStringLiteral("_id"), _id
        },
        {
            QStringLiteral("rid"), rid
        },
        {
            QStringLiteral("starred"), starred
        }
    };
    const QJsonArray params{{
        obj
    }};
    return generateMethod(QStringLiteral("starMessage"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::informTypingStatus(const QString &roomId, const QString &userId, bool typingStatus, quint64 id)
{
    const QString eventName = roomId + QStringLiteral("/typing");
    const QJsonArray params{{
        eventName
    }, {
            userId
        }, {
            typingStatus
        }};
    return generateMethod(QStringLiteral("stream-notify-room"), QJsonDocument(params), id);
}

QJsonValue RocketChatMessage::toJsonDateTime(const QDateTime &dateTime)
{
    if (dateTime.isNull()) {
        return {};
    }

    return QJsonObject{{
        QStringLiteral("$date"), dateTime.toMSecsSinceEpoch()
    }};
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::getSubscriptions(const QDateTime &lastUpdate, quint64 id)
{
    const QJsonArray params{
        toJsonDateTime(lastUpdate)
    };
    return generateMethod(QStringLiteral("subscriptions/get"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setDefaultStatus(User::PresenceStatus status, quint64 id)
{
    const QString strPresence = Utils::presenceStatusToString(status);
    const QJsonArray params{{
        strPresence
    }};
    return generateMethod(QStringLiteral("UserPresence:setDefaultStatus"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setTemporaryStatus(User::PresenceStatus status, quint64 id)
{
    const QString strPresence = Utils::presenceStatusToString(status);
    const QJsonArray params{{}};
    return generateMethod((QStringLiteral("UserPresence:") + strPresence), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::getUsersOfRoom(const QString &roomId, bool showAll, quint64 id)
{
    const QJsonArray params{{
        QJsonValue(roomId), showAll
    }};
    return generateMethod(QStringLiteral("getUsersOfRoom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::createJitsiConfCall(const QString &roomId, quint64 id)
{
    const QJsonArray params{{
        QJsonValue(roomId)
    }};
    return generateMethod(QStringLiteral("jitsi:updateTimeout"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::roomFiles(const QString &roomId, quint64 id)
{
    const QJsonArray params{{
        QJsonValue(roomId)
    }};
    return subscribe(QStringLiteral("roomFiles"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::login(const QString &username, const QString &password, const QString &code, quint64 id)
{
    QJsonObject user;
    user[QStringLiteral("username")] = username;

    QByteArray passwordAsArray = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    const QString hash = QString::fromLatin1(passwordAsArray.toHex());

    QJsonObject passwordObject;
    passwordObject[QStringLiteral("digest")] = hash;
    passwordObject[QStringLiteral("algorithm")] = QStringLiteral("sha-256");

    QJsonObject params;
    if (!code.isEmpty()) {
        QJsonObject loginObject;
        loginObject[QStringLiteral("user")] = user;
        loginObject[QStringLiteral("password")] = passwordObject;
        QJsonObject totpObject;
        totpObject[QStringLiteral("code")] = code;
        totpObject[QStringLiteral("login")] = loginObject;
        params[QStringLiteral("totp")] = totpObject;
    } else {
        params[QStringLiteral("password")] = passwordObject;
        params[QStringLiteral("user")] = user;
    }
    return generateMethod(QStringLiteral("login"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::loginProvider(const QString &credentialToken, const QString &credentialSecretd, quint64 id)
{
    QJsonObject params;
    QJsonObject authKeys;
    authKeys[QStringLiteral("credentialToken")] = credentialToken;
    authKeys[QStringLiteral("credentialSecret")] = credentialSecretd;

    params[QStringLiteral("oauth")] = authKeys;

    return generateMethod(QStringLiteral("login"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::addUserToRoom(const QString &username, const QString &roomId, quint64 id)
{
    //This method use multiusers
    const QJsonArray users = QJsonArray::fromStringList(QStringList() << username);

    QJsonArray params;

    QJsonObject param;
    param[QStringLiteral("rid")] = roomId;
    param[QStringLiteral("username")] = users.first();
    params.append(param);

    return generateMethod(QStringLiteral("addUserToRoom"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::logout(const QString &name, quint64 id)
{
    Q_UNUSED(name);
    //TODO add name
    const QJsonArray params{{}};
    return generateMethod(QStringLiteral("logout"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::userAutocomplete(const QString &searchText, const QString &exception, quint64 id)
{
    QJsonArray params;

    QJsonObject firstParam;

    const QStringList users = exception.split(QLatin1Char(','));
    QJsonArray exceptionEntries;
    for (const QString &entry: users) {
        exceptionEntries.append(entry);
    }
    firstParam[QStringLiteral("exceptions")] = exceptionEntries;
    firstParam[QStringLiteral("term")] = searchText;
    params.append(firstParam);
    return subscribe(QStringLiteral("userAutocomplete"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::channelAndPrivateAutocomplete(const QString &pattern, const QString &exceptions, quint64 id)
{
    return searchRoomUsers(pattern, exceptions, true, true, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::inputChannelAutocomplete(const QString &pattern, const QString &exceptions, quint64 id)
{
    return searchRoomUsers(pattern, exceptions, false, true, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::inputUserAutocomplete(const QString &pattern, const QString &exceptions, quint64 id)
{
    return searchRoomUsers(pattern, exceptions, true, false, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::searchRoomUsers(const QString &pattern, const QString &exceptions, bool searchUser, bool searchRoom, quint64 id)
{
    QJsonArray params;
    params.append(pattern);

    const QJsonArray exceptionJson = QJsonArray::fromStringList(exceptions.split(QLatin1Char(',')));
    params.append(exceptionJson);

    QJsonObject secondParams;
    secondParams[QStringLiteral("rooms")] = searchRoom;
    secondParams[QStringLiteral("users")] = searchUser;
    params.append(secondParams);
    return generateMethod(QStringLiteral("spotlight"), QJsonDocument(params), id);
}

//We need to be able to send file for audio/video
RocketChatMessage::RocketChatMessageResult RocketChatMessage::sendFileMessage(const QJsonObject &result, const QString &serviceUploadType, const QString &roomId, quint64 id)
{
    const QJsonArray obj = {roomId, serviceUploadType, result};
    const QJsonArray params{{
        obj
    }};
    return generateMethod(QStringLiteral("sendFileMessage"), QJsonDocument(params), id);
}

//Verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::unsubscribe(quint64 id)
{
    QJsonObject json;
    json[QStringLiteral("msg")] = QStringLiteral("unsub");
    json[QStringLiteral("id")] = QString::number(id);
    const QString generatedJsonDoc = QString::fromUtf8(QJsonDocument(json).toJson(mJsonFormat));
    RocketChatMessageResult result;
    result.result = generatedJsonDoc;
    return result;
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::subscribe(const QString &name, const QJsonDocument &params, quint64 id)
{
    //TODO fixme.
    QJsonObject json;
    json[QStringLiteral("msg")] = QStringLiteral("sub");
    json[QStringLiteral("id")] = QString::number(id);
    json[QStringLiteral("name")] = name;
    if (params.isArray()) {
        json[QStringLiteral("params")] = params.array();
    } else if (params.isObject()) {
        QJsonArray arr;
        arr.append(params.object());
        json[QStringLiteral("params")] = arr;
    }

    const QString generatedJsonDoc = QString::fromUtf8(QJsonDocument(json).toJson(mJsonFormat));
    RocketChatMessageResult result;
    result.jsonDocument = params;
    result.method = name;
    result.result = generatedJsonDoc;
    return result;
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::generateMethod(const QString &method, const QJsonDocument &params, quint64 id)
{
    QJsonObject json;
    json[QStringLiteral("msg")] = QStringLiteral("method");
    json[QStringLiteral("method")] = method;
    json[QStringLiteral("id")] = QString::number(id);

    if (!params.isEmpty()) {
        if (params.isArray()) {
            json[QStringLiteral("params")] = params.array();
        } else if (params.isObject()) {
            QJsonArray arr;
            arr.append(params.object());
            json[QStringLiteral("params")] = arr;
        }
    }
    const QString generatedJsonDoc = QString::fromUtf8(QJsonDocument(json).toJson(mJsonFormat));
    RocketChatMessageResult result;
    result.jsonDocument = params;
    result.method = method;
    result.result = generatedJsonDoc;
    return result;
}
