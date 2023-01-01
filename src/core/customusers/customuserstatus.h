/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "user.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT CustomUserStatus
{
public:
    CustomUserStatus();
    ~CustomUserStatus();

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &value);

    Q_REQUIRED_RESULT qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT User::PresenceStatus statusType() const;
    void setStatusType(User::PresenceStatus statusType);

    void parseCustomStatus(const QJsonObject &customStatusObj, bool useRestApi = true);

    Q_REQUIRED_RESULT bool operator==(const CustomUserStatus &other) const;

private:
    QString mIdentifier;
    QString mName;
    qint64 mUpdatedAt = -1;
    User::PresenceStatus mStatusType = User::PresenceStatus::Unknown;
};
Q_DECLARE_METATYPE(CustomUserStatus)
Q_DECLARE_TYPEINFO(CustomUserStatus, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomUserStatus &t);
