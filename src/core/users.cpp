/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "users.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

Users::Users()
{
}

bool Users::isEmpty() const
{
    return mUsers.isEmpty();
}

void Users::clear()
{
    mUsers.clear();
}

int Users::count() const
{
    return mUsers.count();
}

User Users::at(int index) const
{
    return mUsers.at(index);
}

void Users::parseMoreUsers(const QJsonObject &obj)
{
    const int usersCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    parseListUsers(obj);
    mUsersCount += usersCount;
}

void Users::parseListUsers(const QJsonObject &obj)
{
    const QJsonArray adminRoomsArray = obj[QStringLiteral("result")].toArray();
    mUsers.reserve(mUsers.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject userObject = current.toObject();
            User m;
            m.parseUserRestApi(userObject);
            mUsers.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing Users" << current;
        }
    }
}

int Users::roomsCount() const
{
    return mUsersCount;
}

void Users::setRoomsCount(int count)
{
    mUsersCount = count;
}

QVector<User> Users::users() const
{
    return mUsers;
}

void Users::setUsers(const QVector<User> &rooms)
{
    mUsers = rooms;
}

void Users::parseUsers(const QJsonObject &obj)
{
    mUsersCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    mUsers.clear();
    parseListUsers(obj);
}

int Users::offset() const
{
    return mOffset;
}

void Users::setOffset(int offset)
{
    mOffset = offset;
}

int Users::total() const
{
    return mTotal;
}

void Users::setTotal(int total)
{
    mTotal = total;
}

QDebug operator<<(QDebug d, const Users &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "usersCount " << t.roomsCount() << "\n";
    for (int i = 0, total = t.users().count(); i < total; ++i) {
        d << t.users().at(i) << "\n";
    }
    return d;
}
