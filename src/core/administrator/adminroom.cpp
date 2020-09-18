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

#include "adminroom.h"

AdminRoom::AdminRoom()
{

}

AdminRoom::~AdminRoom()
{

}

void AdminRoom::parseAdminRoom(const QJsonObject &object)
{
    //TODO
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

void AdminRoom::setChannelType(const QString &channelType)
{
    mChannelType = channelType;
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

bool AdminRoom::operator ==(const AdminRoom &other) const
{
    return mDefaultRoom == other.defaultRoom()
            && mUsersCount == other.usersCount()
            && mMessageCount == other.messageCount()
            && mChannelType == other.channelType()
            && mIdentifier == other.identifier()
            && mTopic == other.topic();
}


QDebug operator <<(QDebug d, const AdminRoom &t)
{
    d << " default Room : " << t.defaultRoom();
    d << " user count : " << t.usersCount();
    d << " message count : " << t.messageCount();
    d << " channel type: " << t.channelType();
    d << " identifier: " << t.identifier();
    d << " topic: " << t.topic();
    return d;
}
