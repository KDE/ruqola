/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "user.h"
#include <QDebug>

class LIBRUQOLACORE_EXPORT CustomUserStatus
{
public:
    CustomUserStatus();
    ~CustomUserStatus();

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QString name() const;
    void setName(const QString &value);

    [[nodiscard]] qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &identifier);

    [[nodiscard]] User::PresenceStatus statusType() const;
    void setStatusType(User::PresenceStatus statusType);

    void parseCustomStatus(const QJsonObject &customStatusObj, bool useRestApi = true);

    [[nodiscard]] bool operator==(const CustomUserStatus &other) const;

private:
    QString mIdentifier;
    QString mName;
    qint64 mUpdatedAt = -1;
    User::PresenceStatus mStatusType = User::PresenceStatus::Unknown;
};
Q_DECLARE_METATYPE(CustomUserStatus)
Q_DECLARE_TYPEINFO(CustomUserStatus, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomUserStatus &t);
