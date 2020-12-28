/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef CUSTOMUSERSTATUS_H
#define CUSTOMUSERSTATUS_H

#include <QDebug>
#include <QJsonObject>
#include "libruqolacore_export.h"
#include "user.h"
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

    Q_REQUIRED_RESULT bool operator ==(const CustomUserStatus &other) const;
private:
    QString mIdentifier;
    QString mName;
    qint64 mUpdatedAt = -1;
    User::PresenceStatus mStatusType = User::PresenceStatus::Unknown;
};
Q_DECLARE_METATYPE(CustomUserStatus)
Q_DECLARE_TYPEINFO(CustomUserStatus, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const CustomUserStatus &t);

#endif // CUSTOMUSERSTATUS_H
