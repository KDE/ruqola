/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatmessage.h"
#include "ruqola_debug.h"
#include "utils.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

RocketChatMessage::RocketChatMessage() = default;

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

RocketChatMessage::RocketChatMessageResult RocketChatMessage::updateOAuthApp(const QString &name, bool active, const QString &redirectUrl, quint64 id)
{
    QJsonObject obj{
        {QStringLiteral("name"), name},
        {QStringLiteral("active"), active},
        {QStringLiteral("redirectUri"), redirectUrl},
    };
    const QJsonArray params{obj};
    return generateMethod(QStringLiteral("updateOAuthApp"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::addOAuthApp(const QString &name, bool active, const QString &redirectUrl, quint64 id)
{
    QJsonObject obj{
        {QStringLiteral("name"), name},
        {QStringLiteral("active"), active},
        {QStringLiteral("redirectUri"), redirectUrl},
    };
    const QJsonArray params{obj};
    return generateMethod(QStringLiteral("addOAuthApp"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::deleteOAuthApp(const QString &appId, quint64 id)
{
    const QJsonArray params{{appId}};
    qDebug() << " DELETE " << appId;
    return generateMethod(QStringLiteral("deleteOAuthApp"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::videoConferenceConfirmed(const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    return generateVideoConferenceAction(QStringLiteral("confirmed"), roomId, callId, userId, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::videoConferenceRejected(const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    return generateVideoConferenceAction(QStringLiteral("rejected"), roomId, callId, userId, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::videoConferenceCall(const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    return generateVideoConferenceAction(QStringLiteral("call"), roomId, callId, userId, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::videoConferenceAccepted(const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    return generateVideoConferenceAction(QStringLiteral("accepted"), roomId, callId, userId, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::generateVideoConferenceAction(const QString &action, const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    QJsonObject actionParamsObj{
        {QStringLiteral("callId"), callId},
        {QStringLiteral("uid"), userId},
        {QStringLiteral("rid"), roomId},
    };
    QJsonObject actionObj{
        {QStringLiteral("action"), action},
        {QStringLiteral("params"), actionParamsObj},
    };

    QString videoConferenceId = roomId;
    videoConferenceId.remove(userId);
    const QJsonArray params{QStringLiteral("%1/video-conference").arg(videoConferenceId), actionObj};
    return generateMethod(QStringLiteral("stream-notify-user"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::licenseGetModules(quint64 id)
{
    const QJsonArray params{{}};
    return generateMethod(QStringLiteral("license:getModules"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::bannerDismiss(const QString &bannerId, quint64 id)
{
    QJsonObject obj{
        {QStringLiteral("id"), bannerId},
    };

    const QJsonArray params{obj};
    return generateMethod(QStringLiteral("banner/dismiss"), QJsonDocument(params), id);
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

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setAdminStatus(const QString &userId, bool admin, quint64 id)
{
    const QJsonArray params{{userId}, {admin}};
    return generateMethod(QStringLiteral("setAdminStatus"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::uploadCustomSound(const QByteArray &sound, quint64 id)
{
    // TODO fix me
    const QJsonArray params{{}};

    return generateMethod(QStringLiteral("uploadCustomSound"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::deleteCustomSound(const QString &identifier, quint64 id)
{
    const QJsonArray params{{identifier}};

    return generateMethod(QStringLiteral("deleteCustomSound"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::listCustomSounds(quint64 id)
{
    const QJsonArray params{{}};

    return generateMethod(QStringLiteral("listCustomSounds"), QJsonDocument(params), id);
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

RocketChatMessage::RocketChatMessageResult RocketChatMessage::getRoomById(const QString &roomId, quint64 id)
{
    const QJsonArray params{{roomId}};
    return generateMethod(QStringLiteral("getRoomById"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::openRoom(const QString &roomId, quint64 id)
{
    const QJsonArray params{{roomId}};
    return generateMethod(QStringLiteral("openRoom"), QJsonDocument(params), id);
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
    params.append(std::move(firstParam));
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
    params.append(std::move(exceptionJson));

    QJsonObject secondParams;
    secondParams[QStringLiteral("rooms")] = searchRoom;
    secondParams[QStringLiteral("users")] = searchUser;
    params.append(std::move(secondParams));
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

RocketChatMessage::RocketChatMessageResult RocketChatMessage::streamNotifyUserOtrEnd(const QString &roomId, const QString &userId, quint64 id)
{
    const QJsonObject endObject{{QStringLiteral("roomId"), roomId}, {QStringLiteral("userId"), userId}};
    QString otrId = roomId;
    otrId = otrId.remove(userId);
    const QJsonArray params{QStringLiteral("%1/otr").arg(otrId), QStringLiteral("end"), endObject};
    return generateMethod(QStringLiteral("stream-notify-user"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::enable2fa(quint64 id)
{
    const QJsonArray params;
    return generateMethod(QStringLiteral("2fa:enable"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::disable2fa(const QString &code, quint64 id)
{
    const QJsonArray params{code};
    return generateMethod(QStringLiteral("2fa:disable"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::regenerateCodes2fa(const QString &code, quint64 id)
{
    const QJsonArray params{code};
    return generateMethod(QStringLiteral("2fa:regenerateCodes"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::validateTempToken2fa(const QString &code, quint64 id)
{
    const QJsonArray params{code};
    return generateMethod(QStringLiteral("2fa:validateTempToken"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKeys, quint64 id)
{
    const QJsonObject endObject{{QStringLiteral("roomId"), QStringLiteral("%1%2").arg(userTo, userFrom)},
                                {QStringLiteral("userId"), userTo},
                                {QStringLiteral("publicKey"), publicKeys}};
    const QJsonArray params{QStringLiteral("%1/otr").arg(userFrom), QStringLiteral("handshake"), endObject};
    return generateMethod(QStringLiteral("stream-notify-user"), QJsonDocument(params), id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::streamNotifyUserOtrAcknowledge(const QString &roomId, const QString &userId, const QString &publicKeys, quint64 id)
{
    const QJsonObject acknowledgeObject{{QStringLiteral("roomId"), roomId}, {QStringLiteral("userId"), userId}, {QStringLiteral("publicKey"), publicKeys}};
    QString otrId = roomId;
    otrId = otrId.remove(userId);
    const QJsonArray params{QStringLiteral("%1/otr").arg(otrId), QStringLiteral("acknowledge"), acknowledgeObject};
    return generateMethod(QStringLiteral("stream-notify-user"), QJsonDocument(params), id);
#if 0
    {\"id\":\"30\",\"method\":\"stream-notify-user\",\"msg\":\"method\",\"params\":[\"4faACeGzSvG7xMcTy/otr\",\"acknowledge\",{\"publicKey\":\"{\\\"crv\\\":\\\"P-256\\\",\\\"ext\\\":true,\\\"key_ops\\\":[],\\\"kty\\\":\\\"EC\\\",\\\"x\\\":\\\"Jg7HgVygchsJSpGc1N36I7-4xlIF2Y4kBB0cKoT5rW8\\\",\\\"y\\\":\\\"rhdmHfXGihoZI0eBL1lADOm3FGrQ3qO6y2rXuV9YNC8\\\"}\",\"roomId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB\",\"userId\":\"4faACeGzSvG7xMcTy\"}]}"
    {\"id\":\"30\",\"method\":\"stream-notify-user\",\"msg\":\"method\",\"params\":[\"4faACeGzSvG7xMcTy/otr\",\"acknowledge\",{\"publicKey\":\"{}\",\"roomId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB4faACeGzSvG7xMcTy\",\"userId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB\"}]}
    {\"msg\":\"method\",\"id\":\"22\",\"method\":\"stream-notify-user\",\"params\":[\"YbwG4T2uB3wZSZSKB/otr\",\"acknowledge\",{\"roomId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB\",\"userId\":\"4faACeGzSvG7xMcTy\",\"publicKey\":\"{
#endif
}

QDebug operator<<(QDebug d, const RocketChatMessage::RocketChatMessageResult &t)
{
    d << "json: " << t.jsonDocument;
    d << "method: " << t.method;
    d << "result: " << t.result;
    return d;
}
