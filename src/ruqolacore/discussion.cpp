/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "discussion.h"

#include <QJsonObject>

Discussion::Discussion()
{
}

QString Discussion::description() const
{
    return mDescription;
}

void Discussion::setDescription(const QString &description)
{
    mDescription = description;
}

QString Discussion::parentRoomId() const
{
    return mParentRoomId;
}

void Discussion::setParentRoomId(const QString &parentRoomId)
{
    mParentRoomId = parentRoomId;
}

int Discussion::numberMessages() const
{
    return mNumberMessages;
}

void Discussion::setNumberMessages(int numberMessages)
{
    mNumberMessages = numberMessages;
}

qint64 Discussion::lastMessage() const
{
    return mLastMessage;
}

void Discussion::setLastMessage(const qint64 &lastMessage)
{
    mLastMessage = lastMessage;
}

QDebug operator <<(QDebug d, const Discussion &t)
{
    d << "Parent Id " << t.parentRoomId();
    d << "Last Message " << t.lastMessage();
    d << "Number of Messages " << t.numberMessages();
    d << "Description " << t.description();
    return d;
}

bool Discussion::operator ==(const Discussion &other) const
{
    return (description() == other.description())
           && (parentRoomId() == other.parentRoomId())
           && (numberMessages() == other.numberMessages())
           && (lastMessage() == other.lastMessage());
}

Discussion &Discussion::operator=(const Discussion &other)
{
    mParentRoomId = other.parentRoomId();
    mDescription = other.description();
    mNumberMessages = other.numberMessages();
    mLastMessage = other.lastMessage();
    return *this;
}

void Discussion::parseDiscussion(const QJsonObject &o)
{
    mParentRoomId = o.value(QLatin1String("prid")).toString();
    mDescription = o.value(QLatin1String("description")).toString();
    mNumberMessages = o.value(QLatin1String("msgs")).toInt();
    //TODO mLastMessage =
}
