/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "channel.h"
#include "utils.h"
#include "ruqola_debug.h"

#include <QJsonObject>

Channel::Channel()
{
}

Channel::~Channel()
{
}

void Channel::parseChannel(const QJsonObject &object, ChannelType type)
{
    mType = type;
    if (mType == ChannelType::PrivateChannel) {
        mUserId = object.value(QLatin1String("_id")).toString();
        mName = object.value(QLatin1String("name")).toString();
        mStatus = object.value(QLatin1String("status")).toString();
        mUserName = object.value(QLatin1String("username")).toString();
    } else {
        mRoomId = object.value(QLatin1String("_id")).toString();
        mRoomName = object.value(QLatin1String("name")).toString();
        mRoomType = object.value(QLatin1String("t")).toString();
    }
}

Channel::ChannelType Channel::type() const
{
    return mType;
}

void Channel::setType(const ChannelType &type)
{
    mType = type;
}

bool Channel::operator==(const Channel &other) const
{
    return (mType == other.type())
            && (mRoomId == other.roomId())
            && (mRoomType == other.roomType())
            && (mRoomName == other.roomName())
            && (mUserId == other.userId())
            && (mName == other.name())
            && (mStatus == other.status())
            && (mUserName == other.userName());
}

QString Channel::roomId() const
{
    return mRoomId;
}

void Channel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString Channel::roomName() const
{
    return mRoomName;
}

void Channel::setRoomName(const QString &roomName)
{
    mRoomName = roomName;
}

QString Channel::roomType() const
{
    return mRoomType;
}

void Channel::setRoomType(const QString &roomType)
{
    mRoomType = roomType;
}

QString Channel::userId() const
{
    return mUserId;
}

void Channel::setUserId(const QString &userId)
{
    mUserId = userId;
}

QString Channel::name() const
{
    return mName;
}

void Channel::setName(const QString &name)
{
    mName = name;
}

QString Channel::status() const
{
    return mStatus;
}

void Channel::setStatus(const QString &status)
{
    mStatus = status;
}

QString Channel::userName() const
{
    return mUserName;
}

void Channel::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString Channel::iconFromStatus() const
{
    return Utils::iconFromStatus(mStatus);
}

QDebug operator <<(QDebug d, const Channel &t)
{
    d << "type: " << t.type();
    d << "roomName: " << t.roomName();
    d << "roomType: " << t.roomType();
    d << "roomId: " << t.roomId();
    d << "mUserId " << t.userId();
    d << "mName " << t.name();
    d << "mStatus " << t.status();
    d << "mUserName " << t.userName();

    return d;
}
