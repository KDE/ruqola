/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "banneduser.h"
#include "libruqola_private_export.h"
#include "paginatedinfolist.h"
class QDebug;

class LIBRUQOLACORE_TESTS_EXPORT BannedUsers : public PaginatedInfoList<BannedUser, &BannedUser::parseBannedUser>
{
public:
    void parseBannedUsers(const QJsonObject &obj);
    void parseMoreBannedUsers(const QJsonObject &obj);

    // Unbanning a user removes them from the server side too, so the total shrinks with the list.
    BannedUser takeAt(int index);
};

QT_DECL_METATYPE_EXTERN_TAGGED(BannedUsers, Ruqola_BannedUsers, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannedUsers &t);
