/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatmessage.h"
#include "utils.h"

#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;

RocketChatMessage::RocketChatMessage() = default;

void RocketChatMessage::setJsonFormat(QJsonDocument::JsonFormat jsonFormat)
{
    mJsonFormat = jsonFormat;
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::openDirectChannel(const QString &username, quint64 id)
{
    const QJsonArray params{QJsonValue(username)};
    return generateMethod(u"createDirectMessage"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::videoConferenceConfirmed(const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    return generateVideoConferenceAction(u"confirmed"_s, roomId, callId, userId, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::videoConferenceRejected(const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    return generateVideoConferenceAction(u"rejected"_s, roomId, callId, userId, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::videoConferenceCall(const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    return generateVideoConferenceAction(u"call"_s, roomId, callId, userId, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::videoConferenceAccepted(const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    return generateVideoConferenceAction(u"accepted"_s, roomId, callId, userId, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::generateVideoConferenceAction(const QString &action, const QString &roomId, const QString &callId, const QString &userId, quint64 id)
{
    const QJsonObject actionParamsObj{
        {u"callId"_s, callId},
        {u"uid"_s, userId},
        {u"rid"_s, roomId},
    };
    const QJsonObject actionObj{
        {u"action"_s, action},
        {u"params"_s, actionParamsObj},
    };

    QString videoConferenceId = roomId;
    videoConferenceId.remove(userId);
    const QJsonArray params{u"%1/video-conference"_s.arg(videoConferenceId), actionObj};
    return generateMethod(u"stream-notify-user"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::bannerDismiss(const QByteArray &bannerId, quint64 id)
{
    const QJsonObject obj{
        {u"id"_s, QString::fromLatin1(bannerId)},
    };

    const QJsonArray params{obj};
    return generateMethod(u"banner/dismiss"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::blockUser(const QString &rid, const QString &userId, quint64 id)
{
    const QJsonObject obj{
        {u"rid"_s, rid},
        {u"blocked"_s, userId},
    };
    const QJsonArray params{{obj}};

    return generateMethod(u"blockUser"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setAdminStatus(const QByteArray &userId, bool admin, quint64 id)
{
    const QJsonArray params{{QString::fromLatin1(userId)}, {admin}};
    return generateMethod(u"setAdminStatus"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::uploadCustomSound(const QByteArray &sound, quint64 id)
{
    // TODO fix me
    const QJsonArray params{{}};

    return generateMethod(u"uploadCustomSound"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::deleteCustomSound(const QByteArray &identifier, quint64 id)
{
    const QJsonArray params{{QString::fromLatin1(identifier)}};

    return generateMethod(u"deleteCustomSound"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::listCustomSounds(quint64 id)
{
    const QJsonArray params{{}};

    return generateMethod(u"listCustomSounds"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::unblockUser(const QString &rid, const QString &userId, quint64 id)
{
    const QJsonObject obj{
        {u"rid"_s, rid},
        {u"blocked"_s, userId},
    };
    const QJsonArray params{{obj}};

    return generateMethod(u"unblockUser"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::getRoomByTypeAndName(const QByteArray &roomId, const QString &roomType, quint64 id)
{
    const QJsonArray params{{roomType}, {QString::fromLatin1(roomId)}};
    return generateMethod(u"getRoomByTypeAndName"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::openRoom(const QByteArray &roomId, quint64 id)
{
    const QJsonArray params{{QString::fromLatin1(roomId)}};
    return generateMethod(u"openRoom"_s, params, id);
}

// Verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::deleteFileMessage(const QByteArray &fileId, quint64 id)
{
    const QJsonArray params{{QString::fromLatin1(fileId)}};
    return generateMethod(u"deleteFileMessage"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::informTypingStatus(const QByteArray &roomId, const QString &userId, bool typingStatus, quint64 id)
{
    const QString eventName = QString::fromLatin1(roomId) + u"/user-activity"_s;
    const QJsonArray params{{eventName}, {userId}, {typingStatus}};
    return generateMethod(u"stream-notify-room"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::setDefaultStatus(User::PresenceStatus status, quint64 id)
{
    const QString strPresence = Utils::presenceStatusToString(status);
    const QJsonArray params{{strPresence}};
    return generateMethod(u"UserPresence:setDefaultStatus"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::createJitsiConfCall(const QByteArray &roomId, quint64 id)
{
    const QJsonArray params{{QJsonValue(QString::fromLatin1(roomId))}};
    return generateMethod(u"jitsi:updateTimeout"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::inputChannelAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, quint64 id)
{
    return searchRoomUsers(roomId, pattern, exceptions, false, true, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::inputUserAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, quint64 id)
{
    return searchRoomUsers(roomId, pattern, exceptions, true, false, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::searchRoomUsers(const QByteArray &roomId, const QString &pattern, const QString &exceptions, bool searchUser, bool searchRoom, quint64 id)
{
    QJsonArray params;
    params.append(pattern);

    const QJsonArray exceptionJson = QJsonArray::fromStringList(exceptions.split(u','));
    params.append(std::move(exceptionJson));

    QJsonObject secondParams;
    if (searchRoom) {
        secondParams["rooms"_L1] = searchRoom;
    }
    if (searchUser) {
        secondParams["users"_L1] = searchUser;
    }
    secondParams["mentions"_L1] = true;
    params.append(std::move(secondParams));
    if (!roomId.isEmpty()) {
        params.append(QString::fromLatin1(roomId));
    }
    return generateMethod(u"spotlight"_s, params, id);
}

// Verify
RocketChatMessage::RocketChatMessageResult RocketChatMessage::unsubscribe(quint64 id)
{
    QJsonObject json;
    json["msg"_L1] = u"unsub"_s;
    json["id"_L1] = QString::number(id);
    const QString generatedJsonDoc = QString::fromUtf8(QJsonDocument(json).toJson(mJsonFormat));
    RocketChatMessageResult result;
    result.result = generatedJsonDoc;
    return result;
}

QJsonObject RocketChatMessage::generateJsonObject(const QString &method, const QJsonObject &params, quint64 id)
{
    QJsonArray arr;
    if (!params.isEmpty()) {
        arr.append(params);
    }
    return generateJsonObject(method, arr, id);
}

QJsonObject RocketChatMessage::generateJsonObject(const QString &method, const QJsonArray &params, quint64 id)
{
    QJsonObject json;
    json["msg"_L1] = u"method"_s;
    json["method"_L1] = method;
    json["id"_L1] = QString::number(id);
    json["params"_L1] = params;
    return json;
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::generateMethod(const QString &method, const QJsonArray &params, quint64 id)
{
    const QJsonObject json = RocketChatMessage::generateJsonObject(method, params, id);
    const QString generatedJsonDoc = QString::fromUtf8(QJsonDocument(json).toJson(mJsonFormat));
    RocketChatMessageResult result;
    result.jsonDocument = QJsonDocument(params);
    result.method = method;
    result.result = generatedJsonDoc;
    return result;
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::generateMethod(const QString &method, const QJsonObject &params, quint64 id)
{
    const QJsonObject json = RocketChatMessage::generateJsonObject(method, params, id);
    const QString generatedJsonDoc = QString::fromUtf8(QJsonDocument(json).toJson(mJsonFormat));
    RocketChatMessageResult result;
    result.jsonDocument = QJsonDocument(params);
    result.method = method;
    result.result = generatedJsonDoc;
    return result;
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::streamNotifyUserOtrEnd(const QString &roomId, const QString &userId, quint64 id)
{
    const QJsonObject endObject{{u"roomId"_s, roomId}, {u"userId"_s, userId}};
    QString otrId = roomId;
    otrId = otrId.remove(userId);
    const QJsonArray params{u"%1/otr"_s.arg(otrId), u"end"_s, endObject};
    return generateMethod(u"stream-notify-user"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::enable2fa(quint64 id)
{
    const QJsonArray params;
    return generateMethod(u"2fa:enable"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::disable2fa(const QString &code, quint64 id)
{
    const QJsonArray params{code};
    return generateMethod(u"2fa:disable"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::regenerateCodes2fa(const QString &code, quint64 id)
{
    const QJsonArray params{code};
    return generateMethod(u"2fa:regenerateCodes"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult RocketChatMessage::validateTempToken2fa(const QString &code, quint64 id)
{
    const QJsonArray params{code};
    return generateMethod(u"2fa:validateTempToken"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKeys, quint64 id)
{
    const QJsonObject endObject{{u"roomId"_s, u"%1%2"_s.arg(userTo, userFrom)}, {u"userId"_s, userTo}, {u"publicKey"_s, publicKeys}};
    const QJsonArray params{u"%1/otr"_s.arg(userFrom), u"handshake"_s, endObject};
    return generateMethod(u"stream-notify-user"_s, params, id);
}

RocketChatMessage::RocketChatMessageResult
RocketChatMessage::streamNotifyUserOtrAcknowledge(const QByteArray &roomId, const QByteArray &userId, const QString &publicKeys, quint64 id)
{
    const QJsonObject acknowledgeObject{{u"roomId"_s, QString::fromLatin1(roomId)}, {u"userId"_s, QString::fromLatin1(userId)}, {u"publicKey"_s, publicKeys}};
    QString otrId = QString::fromLatin1(roomId);
    otrId = otrId.remove(QString::fromLatin1(userId));
    const QJsonArray params{u"%1/otr"_s.arg(otrId), u"acknowledge"_s, acknowledgeObject};
    return generateMethod(u"stream-notify-user"_s, params, id);
#if 0
    {\"id\":\"30\",\"method\":\"stream-notify-user\",\"msg\":\"method\",\"params\":[\"4faACeGzSvG7xMcTy/otr\",\"acknowledge\",{\"publicKey\":\"{\\\"crv\\\":\\\"P-256\\\",\\\"ext\\\":true,\\\"key_ops\\\":[],\\\"kty\\\":\\\"EC\\\",\\\"x\\\":\\\"Jg7HgVygchsJSpGc1N36I7-4xlIF2Y4kBB0cKoT5rW8\\\",\\\"y\\\":\\\"rhdmHfXGihoZI0eBL1lADOm3FGrQ3qO6y2rXuV9YNC8\\\"}\",\"roomId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB\",\"userId\":\"4faACeGzSvG7xMcTy\"}]}"
    {\"id\":\"30\",\"method\":\"stream-notify-user\",\"msg\":\"method\",\"params\":[\"4faACeGzSvG7xMcTy/otr\",\"acknowledge\",{\"publicKey\":\"{}\",\"roomId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB4faACeGzSvG7xMcTy\",\"userId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB\"}]}
    {\"msg\":\"method\",\"id\":\"22\",\"method\":\"stream-notify-user\",\"params\":[\"YbwG4T2uB3wZSZSKB/otr\",\"acknowledge\",{\"roomId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB\",\"userId\":\"4faACeGzSvG7xMcTy\",\"publicKey\":\"{
#endif
}

QDebug operator<<(QDebug d, const RocketChatMessage::RocketChatMessageResult &t)
{
    d.space() << "json:" << t.jsonDocument;
    d.space() << "method:" << t.method;
    d.space() << "result:" << t.result;
    return d;
}

#include "moc_rocketchatmessage.cpp"
