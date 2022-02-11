/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "users.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

Users::Users() = default;

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
    if (index < 0 || index > mUsers.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mUsers.at(index);
}

User &Users::operator[](int i)
{
    return mUsers[i];
}

void Users::parseMoreUsers(const QJsonObject &obj, ParseType type)
{
    const int usersCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    parseListUsers(obj, type);
    mUsersCount += usersCount;
}

void Users::parseListUsers(const QJsonObject &obj, ParseType type)
{
    QString parseTypeStr;
    switch (type) {
    case UserInRoles:
    case Administrator:
        parseTypeStr = QStringLiteral("users");
        break;
    case Directory:
        parseTypeStr = QStringLiteral("result");
        break;
    }

    const QJsonArray adminRoomsArray = obj[parseTypeStr].toArray();
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

void Users::parseUsers(const QJsonObject &obj, ParseType type)
{
    mUsersCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    mUsers.clear();
    parseListUsers(obj, type);
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

User Users::takeAt(int index)
{
    return mUsers.takeAt(index);
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
