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

#include "adminroom.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <QJsonArray>

AdminRoom::AdminRoom()
{
}

AdminRoom::~AdminRoom()
{
}

void AdminRoom::parseAdminRoom(const QJsonObject &object)
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

    // qDebug() << " * this " << *this;
    // Add users "u"
    // TODO
}

bool AdminRoom::defaultRoom() const
{
    return mDefaultRoom;
}

void AdminRoom::setDefaultRoom(bool defaultRoom)
{
    mDefaultRoom = defaultRoom;
}

int AdminRoom::usersCount() const
{
    return mUsersCount;
}

void AdminRoom::setUsersCount(int usersCount)
{
    mUsersCount = usersCount;
}

int AdminRoom::messageCount() const
{
    return mMessageCount;
}

void AdminRoom::setMessageCount(int messageCount)
{
    mMessageCount = messageCount;
}

QString AdminRoom::channelType() const
{
    return mChannelType;
}

static QString convertChannelType(const QString &str)
{
    if (str == QLatin1Char('p')) {
        return i18n("Group");
    } else if (str == QLatin1Char('c')) {
        return i18n("Channel");
    } else if (str == QLatin1Char('d')) {
        return i18n("Direct");
    } else {
        qCWarning(RUQOLA_LOG) << " Unkwnon channel type " << str;
        return str;
    }
}

void AdminRoom::setChannelType(const QString &channelType)
{
    mChannelType = channelType;
    mChannelTypeStr = convertChannelType(channelType);
}

QString AdminRoom::identifier() const
{
    return mIdentifier;
}

void AdminRoom::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QString AdminRoom::topic() const
{
    return mTopic;
}

void AdminRoom::setTopic(const QString &topic)
{
    mTopic = topic;
}

bool AdminRoom::readOnly() const
{
    return mReadOnly;
}

void AdminRoom::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

QString AdminRoom::roomName() const
{
    if (mName.isEmpty()) {
        return mUserNames.join(QStringLiteral(" x "));
    }
    return mName;
}

QString AdminRoom::name() const
{
    return mName;
}

void AdminRoom::setName(const QString &name)
{
    mName = name;
}

QStringList AdminRoom::userNames() const
{
    return mUserNames;
}

void AdminRoom::setUserNames(const QStringList &userNames)
{
    mUserNames = userNames;
}

QStringList AdminRoom::users() const
{
    return mUsers;
}

void AdminRoom::setUsers(const QStringList &users)
{
    mUsers = users;
}

QString AdminRoom::channelTypeStr() const
{
    return mChannelTypeStr;
}

bool AdminRoom::operator==(const AdminRoom &other) const
{
    return mDefaultRoom == other.defaultRoom() && mUsersCount == other.usersCount() && mMessageCount == other.messageCount()
        && mChannelType == other.channelType() && mIdentifier == other.identifier() && mTopic == other.topic() && mName == other.name()
        && mUserNames == other.userNames() && mUsers == other.users();
}

QDebug operator<<(QDebug d, const AdminRoom &t)
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
    return d;
}
