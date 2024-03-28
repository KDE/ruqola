/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelcounterinfo.h"
#include "ruqola_room_memory_debug.h"

ChannelCounterInfo::ChannelCounterInfo()
    : QSharedData()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomExtra created " << this;
}

ChannelCounterInfo::ChannelCounterInfo(const ChannelCounterInfo &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomExtra created " << this;

    mUnreadFrom = other.mUnreadFrom;
    mUnreadMessages = other.mUnreadMessages;
    mMessageCount = other.mMessageCount;
    mUnreadMessageTimeStep = other.mUnreadMessageTimeStep;
    mJoined = other.mJoined;
}

ChannelCounterInfo::~ChannelCounterInfo()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomExtra deleted " << this;
}

void ChannelCounterInfo::parseCounterInfo(const QJsonObject &replyObject)
{
    mMessageCount = replyObject.value(QLatin1StringView("msgs")).toInt();
    mUnreadMessages = replyObject.value(QLatin1StringView("unreads")).toInt();
    mJoined = replyObject.value(QLatin1StringView("joined")).toBool();
    mUnreadFrom = QDateTime::fromString(replyObject.value(QLatin1StringView("unreadsFrom")).toString(), Qt::ISODate);
    mUnreadFrom = mUnreadFrom.toTimeSpec(Qt::UTC);
    mUnreadMessageTimeStep = mUnreadFrom.toMSecsSinceEpoch();
}

bool ChannelCounterInfo::operator==(const ChannelCounterInfo &other) const
{
    return mUnreadMessages == other.unreadMessages() && mMessageCount == other.messageCount() && mUnreadFrom == other.unreadFrom() && mJoined == other.joined();
}

bool ChannelCounterInfo::operator!=(const ChannelCounterInfo &other) const
{
    return !operator==(other);
}

quint64 ChannelCounterInfo::unreadMessages() const
{
    return mUnreadMessages;
}

void ChannelCounterInfo::setUnreadMessages(quint64 unreadMessages)
{
    mUnreadMessages = unreadMessages;
}

QDateTime ChannelCounterInfo::unreadFrom() const
{
    return mUnreadFrom;
}

void ChannelCounterInfo::setUnreadFrom(const QDateTime &unreadFrom)
{
    mUnreadFrom = unreadFrom;
}

quint64 ChannelCounterInfo::messageCount() const
{
    return mMessageCount;
}

void ChannelCounterInfo::setMessageCount(quint64 messageCount)
{
    mMessageCount = messageCount;
}

bool ChannelCounterInfo::joined() const
{
    return mJoined;
}

void ChannelCounterInfo::setJoined(bool joined)
{
    mJoined = joined;
}

bool ChannelCounterInfo::isValid() const
{
    return mUnreadFrom.isValid();
}

qint64 ChannelCounterInfo::unreadMessageTimeStep() const
{
    return mUnreadMessageTimeStep;
}

void ChannelCounterInfo::setUnreadMessageTimeStep(qint64 unreadMessageTimeStep)
{
    mUnreadMessageTimeStep = unreadMessageTimeStep;
}

QDebug operator<<(QDebug d, const ChannelCounterInfo &t)
{
    d.space() << "mUnreadFrom Messages" << t.unreadFrom();
    d.space() << "Unread Messages" << t.unreadMessages();
    d.space() << "Messages count" << t.messageCount();
    d.space() << "Unread from" << t.unreadFrom();
    d.space() << "joined" << t.joined();
    return d;
}
