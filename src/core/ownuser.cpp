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

#include "ownuser.h"

OwnUser::OwnUser()
{
}

OwnUser::~OwnUser()
{
}

void OwnUser::parseOwnUserInfo(const QJsonObject &replyObject)
{
    mUserId = replyObject.value(QLatin1String("_id")).toString();
    mUserName = replyObject.value(QLatin1String("username")).toString();
    mStatus = replyObject.value(QLatin1String("status")).toString();
    mEmail = replyObject.value(QLatin1String("email")).toString();
    mStatusText = replyObject.value(QLatin1String("statusText")).toString();
    mName = replyObject.value(QLatin1String("name")).toString();
}

QString OwnUser::userId() const
{
    return mUserId;
}

void OwnUser::setUserId(const QString &userId)
{
    mUserId = userId;
}

QString OwnUser::userName() const
{
    return mUserName;
}

void OwnUser::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString OwnUser::status() const
{
    return mStatus;
}

void OwnUser::setStatus(const QString &status)
{
    mStatus = status;
}

QDebug operator <<(QDebug d, const OwnUser &t)
{
    d << "UserId " << t.userId();
    d << "Status " << t.status();
    d << "UserName " << t.userName();
    d << "Email " << t.email();
    d << "StatusText " << t.statusText();
    d << "Name " << t.name();
    return d;
}

bool OwnUser::operator ==(const OwnUser &other) const
{
    return (mUserId == other.userId())
           && (mStatus == other.status())
           && (mUserName == other.userName())
            && (mEmail == other.email())
            && (mStatusText == other.statusText())
            && (mName == other.name());
}

QString OwnUser::email() const
{
    return mEmail;
}

void OwnUser::setEmail(const QString &email)
{
    mEmail = email;
}

QString OwnUser::statusText() const
{
    return mStatusText;
}

void OwnUser::setStatusText(const QString &statusText)
{
    mStatusText = statusText;
}

QString OwnUser::name() const
{
    return mName;
}

void OwnUser::setName(const QString &name)
{
    mName = name;
}
