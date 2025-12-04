/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "user.h"
#include <QJsonDocument>
#include <QString>
class LIBRUQOLACORE_TESTS_EXPORT RocketChatMessage
{
    Q_GADGET
public:
    RocketChatMessage();
    ~RocketChatMessage() = default;

    struct RocketChatMessageResult {
        QString method;
        QString result;
        QJsonDocument jsonDocument;
    };

    void setJsonFormat(QJsonDocument::JsonFormat jsonFormat);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult generateMethod(const QString &method, const QJsonArray &params, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult generateMethod(const QString &method, const QJsonObject &params, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult informTypingStatus(const QByteArray &roomId, const QString &userId, bool typingStatus, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult setDefaultStatus(User::PresenceStatus status, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult openDirectChannel(const QString &username, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult createJitsiConfCall(const QByteArray &roomId, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult unsubscribe(quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult blockUser(const QString &rid, const QString &userId, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult unblockUser(const QString &rid, const QString &userId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    searchRoomUsers(const QByteArray &roomId, const QString &pattern, const QString &exceptions, bool searchUser, bool searchRoom, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    inputChannelAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    inputUserAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult deleteFileMessage(const QByteArray &fileId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult streamNotifyUserOtrEnd(const QString &userFrom, const QString &userTo, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKeys, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    streamNotifyUserOtrAcknowledge(const QByteArray &roomId, const QByteArray &userId, const QString &publicKeys, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult deleteCustomSound(const QByteArray &identifier, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult uploadCustomSound(const QByteArray &sound, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult listCustomSounds(quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult enable2fa(quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult disable2fa(const QString &code, quint64 id);
    [[nodiscard]] RocketChatMessage::RocketChatMessageResult validateTempToken2fa(const QString &code, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult regenerateCodes2fa(const QString &code, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult setAdminStatus(const QByteArray &userId, bool admin, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult openRoom(const QByteArray &roomId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult bannerDismiss(const QByteArray &bannerId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    videoConferenceAccepted(const QString &roomId, const QString &callId, const QString &userId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    videoConferenceRejected(const QString &roomId, const QString &callId, const QString &userId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    videoConferenceCall(const QString &roomId, const QString &callId, const QString &userId, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult
    videoConferenceConfirmed(const QString &roomId, const QString &callId, const QString &userId, quint64 id);

    [[nodiscard]] static QJsonObject generateJsonObject(const QString &method, const QJsonObject &params, quint64 id);
    [[nodiscard]] static QJsonObject generateJsonObject(const QString &method, const QJsonArray &params, quint64 id);

    [[nodiscard]] RocketChatMessage::RocketChatMessageResult getRoomByTypeAndName(const QByteArray &roomId, const QString &roomType, quint64 id);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT RocketChatMessage::RocketChatMessageResult
    generateVideoConferenceAction(const QString &action, const QString &roomId, const QString &callId, const QString &userId, quint64 id);

    QJsonDocument::JsonFormat mJsonFormat = QJsonDocument::Compact;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RocketChatMessage::RocketChatMessageResult &t);
