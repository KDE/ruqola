/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "libruqola_private_export.h"
#include "roleinfo.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT Permission
{
public:
    Permission();

    bool parsePermission(const QJsonObject &replyObject, const QVector<RoleInfo> &roleInfo = {});
    Q_REQUIRED_RESULT QStringList roles() const;
    void setRoles(const QStringList &newRoles);

    Q_REQUIRED_RESULT qint64 updatedAt() const;
    void setUpdatedAt(qint64 newUpdatedAt);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    Q_REQUIRED_RESULT const QStringList &rolesStr() const;

private:
    qint64 mUpdatedAt = -1;
    QStringList mRolesStr;
    QStringList mRoles;
    QString mIdentifier;
};
Q_DECLARE_METATYPE(Permission)
Q_DECLARE_TYPEINFO(Permission, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Permission &t);
