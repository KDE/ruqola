/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channel.h"
#include "utils.h"

#include <QJsonObject>

Channel::Channel() = default;

Channel::~Channel() = default;

void Channel::parseChannel(const QJsonObject &object, ChannelType type)
{
    mType = type;
    if (mType == ChannelType::DirectChannel) {
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

void Channel::setType(ChannelType type)
{
    mType = type;
}

bool Channel::operator==(const Channel &other) const
{
    return (mType == other.type()) && (mRoomId == other.roomId()) && (mRoomType == other.roomType()) && (mRoomName == other.roomName())
        && (mUserId == other.userId()) && (mName == other.name()) && (mStatus == other.status()) && (mUserName == other.userName());
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

QDebug operator<<(QDebug d, const Channel &t)
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

#include "moc_channel.cpp"
