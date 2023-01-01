/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "user.h"
#include <QDebug>
#include <QVector>
class LIBRUQOLACORE_EXPORT Users
{
public:
    enum ParseType {
        UserInRoles,
        Administrator,
        Directory,
    };
    Users();

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT User at(int index) const;

    Q_REQUIRED_RESULT User &operator[](int i);

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    User takeAt(int index);

    void parseUsers(const QJsonObject &obj, ParseType type, const QVector<RoleInfo> &roleInfo);
    void parseMoreUsers(const QJsonObject &obj, ParseType type, const QVector<RoleInfo> &roleInfo);

    Q_REQUIRED_RESULT QVector<User> users() const;
    void setUsers(const QVector<User> &rooms);

    Q_REQUIRED_RESULT int usersCount() const;
    void setUsersCount(int adminroomsCount);

    void insertUser(int index, const User &user);
    void appendUser(const User &user);

private:
    void parseListUsers(const QJsonObject &obj, ParseType type, const QVector<RoleInfo> &roleInfo);
    QVector<User> mUsers;
    int mUsersCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(Users)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Users &t);
