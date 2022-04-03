/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationinfo.h"

NotificationInfo::NotificationInfo() = default;

QDebug operator<<(QDebug d, const NotificationInfo &t)
{
    d << " accountName " << t.accountName();
    return d;
}

const QString &NotificationInfo::accountName() const
{
    return mAccountName;
}

void NotificationInfo::setAccountName(const QString &newAccountName)
{
    mAccountName = newAccountName;
}
