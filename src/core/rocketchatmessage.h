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
    streamNotifyUserOtrAcknowledge(const QString &userId, const QString &roomId, const QString &publicKeys, quint64 id);

private:
    Q_DISABLE_COPY(RocketChatMessage)
    Q_REQUIRED_RESULT QJsonValue toJsonDateTime(const QDateTime &dateTime);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult subscribe(const QString &name, const QJsonDocument &params, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult
    saveRoomSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id);
    QJsonDocument::JsonFormat mJsonFormat = QJsonDocument::Compact;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RocketChatMessage::RocketChatMessageResult &t);
