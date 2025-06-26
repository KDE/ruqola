/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "users.h"

#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
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
    if (index < 0 || index >= mUsers.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mUsers.at(index);
}

User &Users::operator[](int i)
{
    return mUsers[i];
}

void Users::parseMoreUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo)
{
    const int usersCount = obj["count"_L1].toInt();
    mOffset = obj["offset"_L1].toInt();
    mTotal = obj["total"_L1].toInt();
    parseListUsers(obj, type, roleInfo);
    mUsersCount += usersCount;
}

void Users::parseListUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo)
{
    QString parseTypeStr;
    switch (type) {
    case ParseType::UserInRoles:
    case ParseType::Administrator:
        parseTypeStr = u"users"_s;
        break;
    case ParseType::Directory:
        parseTypeStr = u"result"_s;
        break;
    }

    const QJsonArray adminRoomsArray = obj[parseTypeStr].toArray();
    mUsers.reserve(mUsers.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject userObject = current.toObject();
            User m;
            m.parseUserRestApi(userObject, roleInfo);
            mUsers.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing Users" << current;
        }
    }
}

int Users::usersCount() const
{
    return mUsersCount;
}

void Users::setUsersCount(int count)
{
    mUsersCount = count;
}

void Users::insertUser(int index, const User &user)
{
    mUsers.insert(index, user);
}

void Users::appendUser(const User &user)
{
    mUsers.append(user);
}

QList<User> Users::users() const
{
    return mUsers;
}

void Users::setUsers(const QList<User> &rooms)
{
    mUsers = rooms;
}

void Users::parseUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo)
{
    mUsersCount = obj["count"_L1].toInt();
    mOffset = obj["offset"_L1].toInt();
    mTotal = obj["total"_L1].toInt();
    mUsers.clear();
    parseListUsers(obj, type, roleInfo);
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
    d.space() << "total " << t.total();
    d.space() << "offset " << t.offset();
    d.space() << "usersCount " << t.usersCount() << "\n";
    for (int i = 0, total = t.users().count(); i < total; ++i) {
        d.space() << t.users().at(i) << "\n";
    }
    return d;
}
