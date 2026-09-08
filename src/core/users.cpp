/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "users.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(Users, Ruqola_Users)

#include "ruqola_debug.h"

User &Users::operator[](int i)
{
    return mList[i];
}

void Users::insertUser(int index, const User &user)
{
    mList.insert(index, user);
}

void Users::appendUser(const User &user)
{
    mList.append(user);
}

void Users::parseUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo)
{
    parseFirstPageCounters(obj);
    parseListUsers(obj, type, roleInfo);
}

void Users::parseMoreUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo)
{
    parseNextPageCounters(obj);
    parseListUsers(obj, type, roleInfo);
}

void Users::parseListUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo)
{
    QLatin1StringView arrayKey;
    switch (type) {
    case ParseType::UserInRoles:
    case ParseType::Administrator:
        arrayKey = QLatin1StringView("users");
        break;
    case ParseType::Directory:
        arrayKey = QLatin1StringView("result");
        break;
    }

    const QJsonArray usersArray = obj[arrayKey].toArray();
    mList.reserve(mList.count() + usersArray.count());
    for (const QJsonValue &current : usersArray) {
        if (current.type() == QJsonValue::Object) {
            mList.emplace_back().parseUserRestApi(current.toObject(), roleInfo);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing Users" << current;
        }
    }
}

QDebug operator<<(QDebug d, const Users &t)
{
    d.space() << "total " << t.total();
    d.space() << "offset " << t.offset();
    d.space() << "usersCount " << t.loadedCount() << "\n";
    for (const User &user : t.list()) {
        d.space() << user << "\n";
    }
    return d;
}
