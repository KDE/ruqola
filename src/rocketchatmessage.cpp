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

#include "rocketchatmessage.h"
#include "ruqola_debug.h"
#include "utils.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

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

//TODO verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::messageSearch(const QString &roomId, const QString &pattern, quint64 id)
{
    const QJsonArray params{
        QJsonValue(pattern),
        QJsonValue(roomId)
    };
    return generateMethod(QStringLiteral("messageSearch"), QJsonDocument(params), id);
}

//TODO verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::createDirectMessage(const QString &username, quint64 id)
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
    const QJsonArray params{{}};
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

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomDescription(const QString &roomId, const QString &description, quint64 id)
{
    return saveRoomSettings(QStringLiteral("roomDescription"), roomId, description, id);
}

/*
RocketChatMessage::RocketChatMessageResult RocketChatMessage::setRoomType(const QString &roomId, QRocketChatRoom::Type type)
{
    switch(type) {
    case RocketChatMessage::DirectChat:
        return saveRoomSettings(roomId, "roomType"_l1, "d"_l1);
    case RocketChatMessage::MultiUserChat:
        return saveRoomSettings(roomId, "roomType"_l1, "c"_l1);
    }
    return {this, RocketChatMessage{"error-invalid-room-type"_l1, i18n("Unsupported room type")}.toMessage()};
}
*/

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

#if 0 //TODO
RocketChatMessage::RocketChatMessageResult RocketChatMessage::updateMessage(const QString &roomId, const QString &accessCode, quint64 id)
{
    const QJsonArray params{{
                                roomId
                            }, {
                                accessCode
                            }};
    return generateMethod(QStringLiteral("joinRoom"), QJsonDocument(params), id);
}

#endif

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

RocketChatMessage::RocketChatMessageResult RocketChatMessage::logout(const QString &name, quint64 id)
{
    Q_UNUSED(name);
    //TODO add name
    const QJsonArray params{{}};
    return generateMethod(QStringLiteral("logout"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::subscribe(const QString &name, const QJsonDocument &params, quint64 id)
{
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
