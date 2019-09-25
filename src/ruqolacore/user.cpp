/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

#include "user.h"
#include "utils.h"
#include "ruqola_debug.h"

#include <QJsonObject>

User::User(QObject *parent)
    : QObject(parent)
{
}

User::~User()
{
}

QString User::name() const
{
    return mName;
}

void User::setName(const QString &name)
{
    if (mName != name) {
        mName = name;
        Q_EMIT nameChanged();
    }
}

QString User::userId() const
{
    return mUserId;
}

void User::setUserId(const QString &userId)
{
    if (mUserId != userId) {
        mUserId = userId;
        Q_EMIT userIdChanged();
    }
}

QString User::status() const
{
    return mStatus;
}

void User::setStatus(const QString &status)
{
    if (mStatus != status) {
        mStatus = status;
        Q_EMIT statusChanged();
    }
}

bool User::operator ==(const User &other) const
{
    return (mName == other.name())
           && (mUserId == other.userId())
           && (mStatus == other.status())
           && (mUserName == other.userName())
           && (mUtcOffset == other.utcOffset())
           && (mStatusText == other.statusText());
}

bool User::operator !=(const User &other) const
{
    return !operator ==(other);
}

QString User::userName() const
{
    return mUserName;
}

void User::setUserName(const QString &userName)
{
    if (mUserName != userName) {
        mUserName = userName;
        Q_EMIT userNameChanged();
    }
}

bool User::isValid() const
{
    return !mUserName.isEmpty() || !mName.isEmpty();
}

double User::utcOffset() const
{
    return mUtcOffset;
}

void User::setUtcOffset(double utcOffset)
{
    if (mUtcOffset != utcOffset) {
        mUtcOffset = utcOffset;
        Q_EMIT utcOffsetChanged();
    }
}

QString User::statusText() const
{
    return mStatusText;
}

void User::setStatusText(const QString &statusText)
{
    if (mStatusText != statusText) {
        mStatusText = statusText;
        Q_EMIT statusTextChanged();
    }
}

QDebug operator <<(QDebug d, const User &t)
{
    d << "Name " << t.name();
    d << "UserId " << t.userId();
    d << "Status " << t.status();
    d << "UserName " << t.userName();
    d << "UtcOffset " << t.utcOffset();
    d << "StatusText " << t.statusText();
    return d;
}

void User::parseUser(const QJsonObject &object)
{
    const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
    setUserId(object.value(QLatin1String("id")).toString());
    setName(fields.value(QLatin1String("name")).toString());
    setStatus(fields.value(QLatin1String("status")).toString());
    setUserName(fields.value(QLatin1String("username")).toString());
    setStatusText(fields.value(QLatin1String("statusText")).toString());
    setUtcOffset(fields.value(QLatin1String("utcOffset")).toDouble());
}

QString User::iconFromStatus() const
{
    return Utils::iconFromStatus(mStatus);
}
