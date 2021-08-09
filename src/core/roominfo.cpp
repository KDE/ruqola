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

#include "roominfo.h"

#include "ruqola_debug.h"
#include "utils.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QLocale>

RoomInfo::RoomInfo()
{
}

RoomInfo::~RoomInfo()
{
}

void RoomInfo::parseRoomInfo(const QJsonObject &object)
{
    // qDebug() << " void AdminRoom::parseAdminRoom(const QJsonObject &object)" << object;
    if (object.contains(QLatin1String("topic"))) {
        setTopic(object[QStringLiteral("topic")].toString());
    }
    if (object.contains(QLatin1String("name"))) {
        setName(object[QStringLiteral("name")].toString());
    }
    if (object.contains(QLatin1String("msgs"))) {
        setMessageCount(object[QStringLiteral("msgs")].toInt());
    }
    if (object.contains(QLatin1String("t"))) {
        setChannelType(object[QStringLiteral("t")].toString());
    }
    if (object.contains(QLatin1String("default"))) {
        setDefaultRoom(object[QStringLiteral("default")].toBool());
    }
    if (object.contains(QLatin1String("lastMessage"))) {
        setLastMessage(Utils::parseIsoDate(QStringLiteral("_updatedAt"), object[QStringLiteral("lastMessage")].toObject()));
    }
    if (object.contains(QLatin1String("ts"))) {
        setCreatedRoom(Utils::parseIsoDate(QStringLiteral("ts"), object));
    }
    setIdentifier(object[QStringLiteral("_id")].toString());
    setReadOnly(object[QStringLiteral("ro")].toBool());
    if (object.contains(QLatin1String("usersCount"))) {
        setUsersCount(object[QStringLiteral("usersCount")].toInt());
    }
    const QJsonArray userNamesArray = object[QStringLiteral("usernames")].toArray();
    const QVariantList userNamesList = userNamesArray.toVariantList();
    mUserNames.reserve(userNamesList.count());
    for (const auto &var : userNamesList) {
        mUserNames.append(var.toString());
    }
    mTeamInfo.parseTeamInfo(object);
    generateDisplayChannelType();
    // TODO load team info
    // qDebug() << " * this " << *this;
    // Add users "u"
    // TODO
}

bool RoomInfo::defaultRoom() const
{
    return mDefaultRoom;
}

void RoomInfo::setDefaultRoom(bool defaultRoom)
{
    mDefaultRoom = defaultRoom;
}

int RoomInfo::usersCount() const
{
    return mUsersCount;
}

void RoomInfo::setUsersCount(int usersCount)
{
    mUsersCount = usersCount;
}

int RoomInfo::messageCount() const
{
    return mMessageCount;
}

void RoomInfo::setMessageCount(int messageCount)
{
    mMessageCount = messageCount;
}

QString RoomInfo::channelType() const
{
    return mChannelType;
}

static QString convertChannelType(const QString &str, bool mainTeam)
{
    if (str == QLatin1Char('p')) {
        if (mainTeam) {
            return i18n("Private Team");
        } else {
            return i18n("Group");
        }
    } else if (str == QLatin1Char('c')) {
        if (mainTeam) {
            return i18n("Public Team");
        } else {
            return i18n("Channel");
        }
    } else if (str == QLatin1Char('d')) {
        return i18n("Direct");
    } else {
        qCWarning(RUQOLA_LOG) << " Unkwnon channel type " << str;
        return str;
    }
}

void RoomInfo::generateDisplayChannelType()
{
    mChannelTypeStr = convertChannelType(mChannelType, mTeamInfo.mainTeam());
}

qint64 RoomInfo::createdRoom() const
{
    return mCreatedRoom;
}

void RoomInfo::setCreatedRoom(qint64 newCreatedRoom)
{
    mCreatedRoom = newCreatedRoom;
    QLocale l;
    mCreatedRoomDisplayTime = (mCreatedRoom != -1) ? l.toString(QDateTime::fromMSecsSinceEpoch(mCreatedRoom), QLocale::LongFormat) : QString();
}

QString RoomInfo::createdRoomDisplayDateTimeStr() const
{
    return mCreatedRoomDisplayTime;
}

qint64 RoomInfo::lastMessage() const
{
    return mLastMessage;
}

QString RoomInfo::lastMessageDisplayDateTimeStr() const
{
    return mLastMessageDisplayTime;
}

void RoomInfo::setLastMessage(qint64 newLastMessage)
{
    mLastMessage = newLastMessage;
    QLocale l;
    mLastMessageDisplayTime = (mLastMessage != -1) ? l.toString(QDateTime::fromMSecsSinceEpoch(mLastMessage), QLocale::LongFormat) : QString();
}

void RoomInfo::setChannelType(const QString &channelType)
{
    mChannelType = channelType;
}

QString RoomInfo::identifier() const
{
    return mIdentifier;
}

void RoomInfo::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QString RoomInfo::topic() const
{
    return mTopic;
}

void RoomInfo::setTopic(const QString &topic)
{
    mTopic = topic;
}

bool RoomInfo::readOnly() const
{
    return mReadOnly;
}

void RoomInfo::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

QString RoomInfo::roomName() const
{
    if (mName.isEmpty()) {
        return mUserNames.join(QStringLiteral(" x "));
    }
    return mName;
}

const TeamInfo &RoomInfo::teamInfo() const
{
    return mTeamInfo;
}

void RoomInfo::setTeamInfo(const TeamInfo &newTeamInfo)
{
    mTeamInfo = newTeamInfo;
}

QString RoomInfo::name() const
{
    return mName;
}

void RoomInfo::setName(const QString &name)
{
    mName = name;
}

QStringList RoomInfo::userNames() const
{
    return mUserNames;
}

void RoomInfo::setUserNames(const QStringList &userNames)
{
    mUserNames = userNames;
}

QStringList RoomInfo::users() const
{
    return mUsers;
}

void RoomInfo::setUsers(const QStringList &users)
{
    mUsers = users;
}

QString RoomInfo::channelTypeStr() const
{
    return mChannelTypeStr;
}

bool RoomInfo::operator==(const RoomInfo &other) const
{
    return mDefaultRoom == other.defaultRoom() && mUsersCount == other.usersCount() && mMessageCount == other.messageCount()
        && mChannelType == other.channelType() && mIdentifier == other.identifier() && mTopic == other.topic() && mName == other.name()
        && mUserNames == other.userNames() && mUsers == other.users() && mTeamInfo == other.teamInfo() && mLastMessage == other.lastMessage()
        && mCreatedRoom == other.createdRoom();
}

QDebug operator<<(QDebug d, const RoomInfo &t)
{
    d << " default Room : " << t.defaultRoom();
    d << " user count : " << t.usersCount();
    d << " message count : " << t.messageCount();
    d << " channel type: " << t.channelType();
    d << " identifier: " << t.identifier();
    d << " topic: " << t.topic();
    d << " name: " << t.name();
    d << " usernames: " << t.userNames();
    d << " users: " << t.users();
    d << " teaminfo: " << t.teamInfo();
    d << " lastMessage : " << t.lastMessage();
    d << " created : " << t.createdRoom();
    return d;
}
