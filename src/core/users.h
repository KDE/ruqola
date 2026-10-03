/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "paginatedinfolist.h"
#include "user.h"
class QDebug;

// User::parseUserRestApi() also needs the server's role list, which PaginatedInfoList knows
// nothing about, so the elements are parsed by parseListUsers() below rather than by
// PaginatedInfoList::parseElements().
class LIBRUQOLACORE_EXPORT Users : public PaginatedInfoListBase<User>
{
public:
    enum class ParseType : uint8_t {
        UserInRoles,
        Administrator,
        Directory,
    };

    [[nodiscard]] User &operator[](int i);

    void parseUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo);
    void parseMoreUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo);

    void insertUser(int index, const User &user);
    void appendUser(const User &user);

private:
    LIBRUQOLACORE_NO_EXPORT void parseListUsers(const QJsonObject &obj, ParseType type, const QList<RoleInfo> &roleInfo);
};

QT_DECL_METATYPE_EXTERN_TAGGED(Users, Ruqola_Users, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Users &t);
