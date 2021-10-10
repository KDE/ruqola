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

#include "rocketchatmessage.h"
#include "ruqola_debug.h"
#include "utils.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

RocketChatMessage::RocketChatMessage()
{
}

void RocketChatMessage::setJsonFormat(QJsonDocument::JsonFormat jsonFormat)
{
    mJsonFormat = jsonFormat;
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::openDirectChannel(const QString &username, quint64 id)
{
    const QJsonArray params{QJsonValue(username)};
    return generateMethod(QStringLiteral("createDirectMessage"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomEncrypted(const QString &roomId, bool encrypted, quint64 id)
{
    return saveRoomSettings(QStringLiteral("encrypted"), roomId, encrypted, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::blockUser(const QString &rid, const QString &userId, quint64 id)
{
    QJsonObject obj{
        {QStringLiteral("rid"), rid},
        {QStringLiteral("blocked"), userId},
    };
    const QJsonArray params{{obj}};

    return generateMethod(QStringLiteral("blockUser"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::unblockUser(const QString &rid, const QString &userId, quint64 id)
{
    QJsonObject obj{
        {QStringLiteral("rid"), rid},
        {QStringLiteral("blocked"), userId},
    };
    const QJsonArray params{{obj}};

    return generateMethod(QStringLiteral("unblockUser"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::saveRoomSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id)
{
    const QJsonArray params{{roomId}, {key}, {value}};
    return generateMethod(QStringLiteral("saveRoomSettings"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::joinRoom(const QString &roomId, const QString &accessCode, quint64 id)
{
    const QJsonArray params{{roomId}, {accessCode}};
    return generateMethod(QStringLiteral("joinRoom"), QJsonDocument(params), id);
}

// Verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::deleteFileMessage(const QString &fileId, quint64 id)
{
    const QJsonArray params{{fileId}};
    return generateMethod(QStringLiteral("deleteFileMessage"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::informTypingStatus(const QString &roomId, const QString &userId, bool typingStatus, quint64 id)
{
    const QString eventName = roomId + QStringLiteral("/typing");
    const QJsonArray params{{eventName}, {userId}, {typingStatus}};
    return generateMethod(QStringLiteral("stream-notify-room"), QJsonDocument(params), id);
}

QJsonValue RocketChatMessage::toJsonDateTime(const QDateTime &dateTime)
{
    if (dateTime.isNull()) {
        return {};
    }

    return QJsonObject{{QStringLiteral("$date"), dateTime.toMSecsSinceEpoch()}};
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setDefaultStatus(User::PresenceStatus status, quint64 id)
{
    const QString strPresence = Utils::presenceStatusToString(status);
    const QJsonArray params{{strPresence}};
    return generateMethod(QStringLiteral("UserPresence:setDefaultStatus"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::roomNameExists(const QString &roomName, quint64 id)
{
    const QJsonArray params{{QJsonValue(roomName)}};
    return generateMethod(QStringLiteral("roomNameExists"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::createJitsiConfCall(const QString &roomId, quint64 id)
{
    const QJsonArray params{{QJsonValue(roomId)}};
    return generateMethod(QStringLiteral("jitsi:updateTimeout"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::userAutocomplete(const QString &searchText, const QString &exception, quint64 id)
{
    QJsonArray params;

    QJsonObject firstParam;

    const QStringList users = exception.split(QLatin1Char(','));
    QJsonArray exceptionEntries;
    for (const QString &entry : users) {
        exceptionEntries.append(entry);
    }
    firstParam[QStringLiteral("exceptions")] = exceptionEntries;
    firstParam[QStringLiteral("term")] = searchText;
    params.append(firstParam);
    return subscribe(QStringLiteral("userAutocomplete"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::inputChannelAutocomplete(const QString &pattern, const QString &exceptions, quint64 id)
{
    return searchRoomUsers(pattern, exceptions, false, true, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::inputUserAutocomplete(const QString &pattern, const QString &exceptions, quint64 id)
{
    return searchRoomUsers(pattern, exceptions, true, false, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::searchRoomUsers(const QString &pattern, const QString &exceptions, bool searchUser, bool searchRoom, quint64 id)
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

// Verify
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
    // TODO fixme.
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

RocketChatMessage::RocketChatMessageResult RocketChatMessage::streamNotifyUserOtrEnd(const QString &userFrom, const QString &userTo, quint64 id)
{
    const QJsonObject endObject{{QStringLiteral("roomId"), QStringLiteral("%1%2").arg(userTo, userFrom)}, {QStringLiteral("userId"), userTo}};

    const QJsonArray params{QStringLiteral("%1/otr").arg(userFrom), QStringLiteral("end"), endObject};
    return generateMethod(QStringLiteral("stream-notify-user"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKeys, quint64 id)
{
    // TODO use publickeys
    const QJsonObject endObject{{QStringLiteral("roomId"), QStringLiteral("%1%2").arg(userTo, userFrom)}, {QStringLiteral("userId"), userTo}};

    const QJsonArray params{QStringLiteral("%1/otr").arg(userFrom), QStringLiteral("handshake"), endObject};
    return generateMethod(QStringLiteral("stream-notify-user"), QJsonDocument(params), id);
}
