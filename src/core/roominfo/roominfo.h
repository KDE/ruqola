/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "teams/teaminfo.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT RoomInfo
{
public:
    RoomInfo();
    ~RoomInfo();
    [[nodiscard]] bool operator==(const RoomInfo &other) const;
    void parseRoomInfo(const QJsonObject &object);

    [[nodiscard]] bool defaultRoom() const;
    void setDefaultRoom(bool defaultRoom);

    [[nodiscard]] int usersCount() const;
    void setUsersCount(int usersCount);

    [[nodiscard]] int messageCount() const;
    void setMessageCount(int messageCount);

    [[nodiscard]] QString channelType() const;
    void setChannelType(const QString &channelType);

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &identifier);

    [[nodiscard]] QString topic() const;
    void setTopic(const QString &topic);

    [[nodiscard]] bool readOnly() const;
    void setReadOnly(bool readOnly);

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QStringList userNames() const;
    void setUserNames(const QStringList &userNames);

    [[nodiscard]] QStringList users() const;
    void setUsers(const QStringList &users);

    [[nodiscard]] QString channelTypeStr() const;

    [[nodiscard]] QString roomName() const;

    [[nodiscard]] const TeamInfo &teamInfo() const;
    void setTeamInfo(const TeamInfo &newTeamInfo);

    [[nodiscard]] qint64 lastMessage() const;
    void setLastMessage(qint64 newLastMessage);

    [[nodiscard]] QString lastMessageDisplayDateTimeStr() const;

    [[nodiscard]] qint64 createdRoom() const;
    void setCreatedRoom(qint64 newCreatedRoom);

    [[nodiscard]] QString createdRoomDisplayDateTimeStr() const;

    [[nodiscard]] bool featured() const;
    void setFeatured(bool newFeatured);

    [[nodiscard]] const QString &description() const;
    void setDescription(const QString &newDescription);

    [[nodiscard]] const QString &announcement() const;
    void setAnnouncement(const QString &newAnnouncement);

    [[nodiscard]] const QString &ownerName() const;
    void setOwnerName(const QString &newOwnerName);

    [[nodiscard]] bool isTeam() const;

    [[nodiscard]] QString belongsTo() const;
    void setBelongsTo(const QString &newBelongsTo);

private:
    LIBRUQOLACORE_NO_EXPORT void generateDisplayChannelType();
    TeamInfo mTeamInfo;
    QStringList mUserNames;
    QStringList mUsers;
    QString mTopic;
    QString mIdentifier;
    QString mChannelType;
    QString mChannelTypeStr;
    QString mName;
    QString mLastMessageDisplayTime;
    QString mCreatedRoomDisplayTime;
    QString mDescription;
    QString mAnnouncement;
    QString mOwnerName;
    QString mBelongsTo;
    qint64 mLastMessage = -1;
    qint64 mCreatedRoom = -1;
    int mMessageCount = -1;
    int mUsersCount = -1;
    bool mDefaultRoom = false;
    bool mReadOnly = false;
    bool mFeatured = false;
};
Q_DECLARE_METATYPE(RoomInfo)
Q_DECLARE_TYPEINFO(RoomInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RoomInfo &t);
