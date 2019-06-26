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

#ifndef ROCKETCHATMESSAGE_H
#define ROCKETCHATMESSAGE_H

#include "libruqola_private_export.h"
#include "user.h"
#include <QString>
#include <QJsonDocument>
class QDateTime;
class LIBRUQOLACORE_TESTS_EXPORT RocketChatMessage
{
    Q_GADGET
public:
    RocketChatMessage();
    ~RocketChatMessage() = default;

    struct RocketChatMessageResult
    {
        QString method;
        QString result;
        QJsonDocument jsonDocument;
    };

    void setJsonFormat(const QJsonDocument::JsonFormat &jsonFormat);

    Q_REQUIRED_RESULT RocketChatMessageResult hideRoom(const QString &roomID, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessageResult leaveRoom(const QString &roomID, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessageResult openRoom(const QString &roomID, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessageResult unarchiveRoom(const QString &roomID, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessageResult archiveRoom(const QString &roomID, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessageResult eraseRoom(const QString &roomID, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessageResult getRoomRoles(const QString &roomID, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessageResult toggleFavorite(const QString &roomId, bool favorite, quint64 id);

    //save room settings
    Q_REQUIRED_RESULT RocketChatMessageResult setRoomName(const QString &roomId, const QString &name, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessageResult generateMethod(const QString &method, const QJsonDocument &params, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomIsReadOnly(const QString &roomId, bool readOnly, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomHasSystemMessages(const QString &roomId, bool systemMessages, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomIsDefault(const QString &roomId, bool isDefault, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomJoinCode(const QString &roomId, const QString &joinCode, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomTopic(const QString &roomId, const QString &topic, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomDescription(const QString &roomId, const QString &description, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomEncrypted(const QString &roomId, bool encrypted, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult readMessages(const QString &roomID, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult joinRoom(const QString &roomId, const QString &accessCode, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult informTypingStatus(const QString &roomId, const QString &userId, bool typingStatus, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult getSubscriptions(const QDateTime &lastUpdate, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setDefaultStatus(User::PresenceStatus status, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setTemporaryStatus(User::PresenceStatus status, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult getUsersOfRoom(const QString &roomId, bool showAll, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult logout(const QString &name, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult createChannel(const QString &roomName, const QStringList &userList, bool readOnly, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult createPrivateGroup(const QString &roomName, const QStringList &userList, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult deleteMessage(const QString &messageId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult starMessage(const QString &_id, const QString &rid, bool starred, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setReaction(const QString &emoji, const QString &messageId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult openDirectChannel(const QString &username, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult messageSearch(const QString &roomId, const QString &pattern, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult listEmojiCustom(quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult createJitsiConfCall(const QString &roomId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult sendFileMessage(const QJsonObject &result, const QString &serviceUploadType, const QString &roomId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult userAutocomplete(const QString &searchText, const QString &exception, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult updateMessage(quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult unsubscribe(quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomAnnouncement(const QString &roomId, const QString &announcement, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult blockUser(const QString &rid, const QString &userId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult unblockUser(const QString &rid, const QString &userId, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult channelAndPrivateAutocomplete(const QString &pattern, const QString &exceptions, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult roomFiles(const QString &roomId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult searchRoomUsers(const QString &pattern, const QString &exceptions, bool searchUser, bool searchRoom, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult addUserToRoom(const QString &username, const QString &roomId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult login(const QString &username, const QString &password, const QString& code, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult inputChannelAutocomplete(const QString &pattern, const QString &exceptions, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult inputUserAutocomplete(const QString &pattern, const QString &exceptions, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult loginProvider(const QString &credentialToken, const QString &credentialSecretd, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult disableNotifications(const QString &roomId, bool disabled, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult hideUnreadStatus(const QString &roomId, bool disabled, quint64 id);

    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult audioNotifications(const QString &roomId, const QString &value, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult mobilePushNotifications(const QString &roomId, const QString &value, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult desktopNotifications(const QString &roomId, const QString &value, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult emailNotifications(const QString &roomId, const QString &value, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult unreadAlert(const QString &roomId, const QString &value, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult deleteFileMessage(const QString &fileId, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult setRoomType(const QString &roomId, bool privateChannel, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult ignoreUser(const QString &roomId, const QString &userId, bool ignore, quint64 id);
private:
    Q_DISABLE_COPY(RocketChatMessage)
    QJsonValue toJsonDateTime(const QDateTime &dateTime);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult subscribe(const QString &name, const QJsonDocument &params, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult saveRoomSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id);
    Q_REQUIRED_RESULT RocketChatMessage::RocketChatMessageResult saveNotificationsSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id);
    QJsonDocument::JsonFormat mJsonFormat;
};

#endif // ROCKETCHATMESSAGE_H
