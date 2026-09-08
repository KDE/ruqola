/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannedusers.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(BannedUsers, Ruqola_BannedUsers)

using namespace Qt::Literals::StringLiterals;

void BannedUsers::parseBannedUsers(const QJsonObject &obj)
{
    parseInfos(obj, "bannedUsers"_L1);
}

void BannedUsers::parseMoreBannedUsers(const QJsonObject &obj)
{
    parseMoreInfos(obj, "bannedUsers"_L1);
}

BannedUser BannedUsers::takeAt(int index)
{
    --mTotal;
    return PaginatedInfoList::takeAt(index);
}

QDebug operator<<(QDebug d, const BannedUsers &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "bannedUsersCount" << t.loadedCount() << "\n";
    for (const BannedUser &bannedUser : t.list()) {
        d.space() << bannedUser << "\n";
    }
    return d;
}
