/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "role.h"
#include "ruqola_debug.h"

#include <QJsonArray>

Role::Role()
{
}

QString Role::userId() const
{
    return mUserId;
}

void Role::setUserId(const QString &userId)
{
    mUserId = userId;
}

bool Role::isModerator() const
{
    return mIsModerator;
}

void Role::setIsModerator(bool isModerator)
{
    mIsModerator = isModerator;
}

bool Role::isLeader() const
{
    return mIsLeader;
}

void Role::setIsLeader(bool isLeader)
{
    mIsLeader = isLeader;
}

bool Role::isOwner() const
{
    return mIsOwner;
}

void Role::setIsOwner(bool isOwner)
{
    mIsOwner = isOwner;
}

bool Role::operator ==(const Role &other) const
{
    return (mIsOwner == other.isOwner())
           && (mIsModerator == other.isModerator())
           && (mIsLeader == other.isLeader())
           && (mUserId == other.userId());
}

bool Role::isValid() const
{
    return !mUserId.isEmpty();
}

void Role::updateRole(const QString &str, bool b)
{
    if (str == QLatin1String("moderator")) {
        mIsModerator = b;
    } else if (str == QLatin1String("leader")) {
        mIsLeader = b;
    } else if (str == QLatin1String("owner")) {
        mIsOwner = b;
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown role" << str;
    }
}

bool Role::hasARole() const
{
    return mIsOwner || mIsModerator || mIsLeader;
}

void Role::parseRole(const QJsonObject &obj)
{
    mUserId = obj.value(QLatin1String("u")).toObject().value(QLatin1String("_id")).toString();
    const QJsonArray roleArray = obj[QStringLiteral("roles")].toArray();
    for (int i = 0; i < roleArray.count(); ++i) {
        const QString str = roleArray.at(i).toString();
        updateRole(str, true);
    }
}

QDebug operator <<(QDebug d, const Role &t)
{
    d << "UserId: " << t.userId();
    d << "IsLeader: " << t.isLeader();
    d << "IsOwner: " << t.isOwner();
    d << "IsModerator: " << t.isModerator();
    return d;
}
