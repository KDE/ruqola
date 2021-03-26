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

#include "channelcounterinfo.h"

ChannelCounterInfo::ChannelCounterInfo()
{
}

ChannelCounterInfo::~ChannelCounterInfo()
{
}

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

void ChannelCounterInfo::setUnreadMessageTimeStep(const qint64 &unreadMessageTimeStep)
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
