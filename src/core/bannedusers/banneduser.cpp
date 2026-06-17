/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "banneduser.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(BannedUser, Ruqola_BannedUser)
using namespace Qt::Literals::StringLiterals;
BannedUser::BannedUser() = default;

BannedUser::~BannedUser() = default;

QString BannedUser::userName() const
{
    return mUserName;
}

void BannedUser::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QString BannedUser::name() const
{
    return mName;
}

void BannedUser::setName(const QString &newName)
{
    mName = newName;
}

QDebug operator<<(QDebug d, const BannedUser &t)
{
    d.space() << "name" << t.name();
    d.space() << "userName" << t.userName();
    return d;
}
