/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
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

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] User at(int index) const;

    [[nodiscard]] User &operator[](int i);

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    User takeAt(int index);

    void parseUsers(const QJsonObject &obj, ParseType type, const QVector<RoleInfo> &roleInfo);
    void parseMoreUsers(const QJsonObject &obj, ParseType type, const QVector<RoleInfo> &roleInfo);

    [[nodiscard]] QVector<User> users() const;
    void setUsers(const QVector<User> &rooms);

    [[nodiscard]] int usersCount() const;
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
