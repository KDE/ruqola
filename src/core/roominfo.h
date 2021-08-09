/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "libruqolacore_export.h"
#include "teaminfo.h"
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

private:
    void generateDisplayChannelType();
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
    qint64 mLastMessage = -1;
    qint64 mCreatedRoom = -1;
    int mMessageCount = -1;
    int mUsersCount = -1;
    bool mDefaultRoom = false;
    bool mReadOnly = false;
};
Q_DECLARE_METATYPE(RoomInfo)
Q_DECLARE_TYPEINFO(RoomInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RoomInfo &t);
