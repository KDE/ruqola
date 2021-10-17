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

#include "permission.h"

#include "utils.h"

Permission::Permission()
{
}

bool Permission::parsePermission(const QJsonObject &replyObject, const QVector<RoleInfo> &roleInfo)
{
    // Don't store settings value.
    if (!replyObject.value(QLatin1String("settingId")).toString().isEmpty()) {
        return false;
    }
    mIdentifier = replyObject.value(QLatin1String("_id")).toString();
    mUpdatedAt = Utils::parseIsoDate(QStringLiteral("_updatedAt"), replyObject);
    const QJsonArray roleArray = replyObject.value(QLatin1String("roles")).toArray();
    mRoles.reserve(roleArray.count());
    for (int i = 0; i < roleArray.count(); ++i) {
        const QString role{roleArray.at(i).toString()};
        mRoles.append(role);
        for (const RoleInfo &info : roleInfo) {
            if (role == info.identifier()) {
                mRolesStr.append(info.name());
                break;
            }
        }
    }
    return true;
}

QStringList Permission::roles() const
{
    return mRoles;
}

void Permission::setRoles(const QStringList &newRoles)
{
    mRoles = newRoles;
}

qint64 Permission::updatedAt() const
{
    return mUpdatedAt;
}

void Permission::setUpdatedAt(qint64 newUpdatedAt)
{
    mUpdatedAt = newUpdatedAt;
}

bool Permission::isValid() const
{
    return mUpdatedAt != -1;
}

const QString &Permission::identifier() const
{
    return mIdentifier;
}

void Permission::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

const QStringList &Permission::rolesStr() const
{
    return mRolesStr;
}

QDebug operator<<(QDebug d, const Permission &t)
{
    d << "roles : " << t.roles();
    d << "rolesStr : " << t.rolesStr();
    d << "mUpdatedAt " << t.updatedAt();
    d << "mIdentifier " << t.identifier();
    return d;
}
