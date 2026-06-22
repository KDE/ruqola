/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "banneduser.h"
#include "libruqola_private_export.h"
#include <QList>
class QDebug;
class DownloadAppsLanguagesManager;
class LIBRUQOLACORE_TESTS_EXPORT BannedUsers
{
public:
    BannedUsers();

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] BannedUser at(int index) const;

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    void parseBannedUsers(const QJsonObject &commandsObj);
    void parseMoreBannedUsers(const QJsonObject &commandsObj);

    [[nodiscard]] QList<BannedUser> bannedUsers() const;
    void setBannedUsers(const QList<BannedUser> &commands);

    [[nodiscard]] int bannedUsersCount() const;
    void setBannedUsersCount(int commandsCount);

    [[nodiscard]] bool operator==(const BannedUsers &other) const;

    BannedUser takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseListBannedUsers(const QJsonObject &commandsObj);
    QList<BannedUser> mBannedUsers;
    int mBannedUsersCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

QT_DECL_METATYPE_EXTERN_TAGGED(BannedUsers, Ruqola_BannedUsers, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannedUsers &t);
