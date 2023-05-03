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
    Q_REQUIRED_RESULT bool operator==(const RoomInfo &other) const;
    void parseRoomInfo(const QJsonObject &object);

    Q_REQUIRED_RESULT bool defaultRoom() const;
    void setDefaultRoom(bool defaultRoom);

    Q_REQUIRED_RESULT int usersCount() const;
    void setUsersCount(int usersCount);

    Q_REQUIRED_RESULT int messageCount() const;
    void setMessageCount(int messageCount);

    Q_REQUIRED_RESULT QString channelType() const;
    void setChannelType(const QString &channelType);

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT QString topic() const;
    void setTopic(const QString &topic);

    Q_REQUIRED_RESULT bool readOnly() const;
    void setReadOnly(bool readOnly);

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QStringList userNames() const;
    void setUserNames(const QStringList &userNames);

    Q_REQUIRED_RESULT QStringList users() const;
    void setUsers(const QStringList &users);

    Q_REQUIRED_RESULT QString channelTypeStr() const;

    Q_REQUIRED_RESULT QString roomName() const;

    Q_REQUIRED_RESULT const TeamInfo &teamInfo() const;
    void setTeamInfo(const TeamInfo &newTeamInfo);

    Q_REQUIRED_RESULT qint64 lastMessage() const;
    void setLastMessage(qint64 newLastMessage);

    Q_REQUIRED_RESULT QString lastMessageDisplayDateTimeStr() const;

    Q_REQUIRED_RESULT qint64 createdRoom() const;
    void setCreatedRoom(qint64 newCreatedRoom);

    Q_REQUIRED_RESULT QString createdRoomDisplayDateTimeStr() const;

    Q_REQUIRED_RESULT bool featured() const;
    void setFeatured(bool newFeatured);

    Q_REQUIRED_RESULT const QString &description() const;
    void setDescription(const QString &newDescription);

    Q_REQUIRED_RESULT const QString &announcement() const;
    void setAnnouncement(const QString &newAnnouncement);

    Q_REQUIRED_RESULT const QString &ownerName() const;
    void setOwnerName(const QString &newOwnerName);

    Q_REQUIRED_RESULT bool isTeam() const;

    Q_REQUIRED_RESULT QString belongsTo() const;
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
