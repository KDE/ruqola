/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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
    mUserId = userId;
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
           && (mListRooms == other.listRooms());
}

QStringList User::listRooms() const
{
    return mListRooms;
}

void User::setListRooms(const QStringList &listRooms)
{
    if (mListRooms != listRooms) {
        mListRooms = listRooms;
        Q_EMIT listRoomsChanged();
    }
}

QDebug operator <<(QDebug d, const User &t)
{
    d << "Name " << t.name();
    d << "UserId " << t.userId();
    d << "Status " << t.status();
    d << "List Rooms " << t.listRooms();
    return d;
}

void User::parseUser(const QJsonObject &object)
{
    const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
    setName(fields.value(QLatin1String("name")).toString());
    setUserId(object.value(QLatin1String("id")).toString());
    setStatus(fields.value(QLatin1String("status")).toString());
}

QString User::iconFromStatus() const
{
    qCDebug(RUQOLA_LOG) << " QString User::iconFromStatus() const" << name() << " mStatus" << mStatus;
    //TODO optimization ?
    const PresenceStatus status = Utils::presenceStatusFromString(mStatus);
    switch (status) {
    case PresenceStatus::PresenceOnline:
        return QStringLiteral("user-online");
    case PresenceStatus::PresenceBusy:
        return QStringLiteral("user-busy");
    case PresenceStatus::PresenceAway:
        return QStringLiteral("user-away");
    case PresenceStatus::PresenceOffline:
        return QStringLiteral("user-offline");
    case PresenceStatus::Unknown:
        return QStringLiteral("unknown");
    }
    return {};
}
