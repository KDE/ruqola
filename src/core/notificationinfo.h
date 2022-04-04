/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT NotificationInfo
{
public:
    NotificationInfo();
    ~NotificationInfo() = default;

    Q_REQUIRED_RESULT const QString &accountName() const;
    void setAccountName(const QString &newAccountName);

private:
    QString mAccountName;
};
Q_DECLARE_TYPEINFO(NotificationInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const NotificationInfo &t);
