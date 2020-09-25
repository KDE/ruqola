/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
    //TODO
}

bool ChannelCounterInfo::operator ==(const ChannelCounterInfo &other) const
{
    return mUnreadMessages == other.unreadMessages()
            && mMessageCount == other.messageCount()
            && mUnreadFrom == other.unreadFrom()
            && mJoined == other.joined();
}

uint ChannelCounterInfo::unreadMessages() const
{
    return mUnreadMessages;
}

void ChannelCounterInfo::setUnreadMessages(uint unreadMessages)
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

uint ChannelCounterInfo::messageCount() const
{
    return mMessageCount;
}

void ChannelCounterInfo::setMessageCount(const uint &messageCount)
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

QDebug operator <<(QDebug d, const ChannelCounterInfo &t)
{
    d << "Unread Messages " << t.unreadMessages();
    d << "Messages count " << t.messageCount();
    d << "Unread from " << t.unreadFrom();
    d << "joined " << t.joined();
    return d;
}
