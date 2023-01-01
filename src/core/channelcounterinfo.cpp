/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelcounterinfo.h"

ChannelCounterInfo::ChannelCounterInfo() = default;

ChannelCounterInfo::~ChannelCounterInfo() = default;

void ChannelCounterInfo::parseCounterInfo(const QJsonObject &replyObject)
{
    mMessageCount = replyObject.value(QLatin1String("msgs")).toInt();
    mUnreadMessages = replyObject.value(QLatin1String("unreads")).toInt();
    mJoined = replyObject.value(QLatin1String("joined")).toBool();
    mUnreadFrom = QDateTime::fromString(replyObject.value(QLatin1String("unreadsFrom")).toString(), Qt::ISODate);
    mUnreadFrom = mUnreadFrom.toTimeSpec(Qt::LocalTime);
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
    d << "Unread Messages " << t.unreadMessages();
    d << "Messages count " << t.messageCount();
    d << "Unread from " << t.unreadFrom();
    d << "joined " << t.joined();
    return d;
}
