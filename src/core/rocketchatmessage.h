/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "user.h"
#include <QDebug>
#include <QJsonDocument>
#include <QString>
class QDateTime;
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

    Q_REQUIRED_RESULT RocketChatMessageResult generateMethod(const QString &method, const QJsonDocument &params, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomEncrypted(const QString &roomId, bool encrypted, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult joinRoom(const QString &roomId, const QString &accessCode, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult
    informTypingStatus(const QString &roomId, const QString &userId, bool typingStatus, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setDefaultStatus(User::PresenceStatus status, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult openDirectChannel(const QString &username, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult createJitsiConfCall(const QString &roomId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult userAutocomplete(const QString &searchText, const QString &exception, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult unsubscribe(quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult blockUser(const QString &rid, const QString &userId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult unblockUser(const QString &rid, const QString &userId, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult
    searchRoomUsers(const QString &pattern, const QString &exceptions, bool searchUser, bool searchRoom, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult inputChannelAutocomplete(const QString &pattern, const QString &exceptions, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult inputUserAutocomplete(const QString &pattern, const QString &exceptions, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult deleteFileMessage(const QString &fileId, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult roomNameExists(const QString &roomName, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult streamNotifyUserOtrEnd(const QString &userFrom, const QString &userTo, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult
    streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKeys, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult
    streamNotifyUserOtrAcknowledge(const QString &roomId, const QString &userId, const QString &publicKeys, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult deleteCustomSound(const QString &identifier, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult uploadCustomSound(const QByteArray &sound, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult listCustomSounds(quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult enable2fa(quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult disable2fa(const QString &code, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult validateTempToken2fa(const QString &code, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult regenerateCodes2fa(const QString &code, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult deleteOAuthApp(const QString &appId, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult addOAuthApp(const QString &name, bool active, const QString &redirectUrl, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult updateOAuthApp(const QString &name, bool active, const QString &redirectUrl, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setAdminStatus(const QString &userId, bool admin, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult openRoom(const QString &roomId, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult getRoomById(const QString &roomId, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult bannerDismiss(const QString &bannerId, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult licenseGetModules(quint64 id);

private:
    Q_DISABLE_COPY(RocketChatMessage)
    Q_REQUIRED_RESULT QJsonValue toJsonDateTime(const QDateTime &dateTime);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult subscribe(const QString &name, const QJsonDocument &params, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult
    saveRoomSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id);
    QJsonDocument::JsonFormat mJsonFormat = QJsonDocument::Compact;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RocketChatMessage::RocketChatMessageResult &t);
