/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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
#include "ruqola_debug.h"

Channel::Channel()
{
}

Channel::~Channel()
{
}

void Channel::parseChannel(const QJsonObject &object)
{
    //TODO
}

Channel::ChannelType Channel::type() const
{
    return mType;
}

void Channel::setType(const ChannelType &type)
{
    mType = type;
}

User Channel::user() const
{
    return mUser;
}

void Channel::setUser(const User &user)
{
    mUser = user;
}

bool Channel::operator==(const Channel &other) const
{
    return (mType == other.type()) && (mUser == other.user());
}

Channel &Channel::operator=(const Channel &other)
{
    mType = other.type();
    mUser = other.user();
    return *this;
}

QDebug operator <<(QDebug d, const Channel &t)
{
    d << "type: " << t.type();
    //TODO
    return d;
}
