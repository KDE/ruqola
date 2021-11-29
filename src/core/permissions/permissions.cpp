/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissions.h"
#include "ruqola_debug.h"

#include <QJsonArray>

Permissions::Permissions() = default;

QVector<Permission> Permissions::permissions() const
{
    return mPermissions;
}

int Permissions::count() const
{
    return mPermissions.count();
}

void Permissions::clear()
{
    mPermissions.clear();
}

Permission Permissions::at(int index) const
{
    return mPermissions.at(index);
}

bool Permissions::isEmpty() const
{
    return mPermissions.isEmpty();
}

void Permissions::setPermissions(const QVector<Permission> &roles)
{
    mPermissions = roles;
}

void Permissions::parsePermissions(const QJsonObject &obj, const QString &str, const QVector<RoleInfo> &roleInfo)
{
    mPermissions.clear();

    const QJsonArray permissionArray = obj[str.isEmpty() ? QStringLiteral("update") : str].toArray();
    const int roleArrayCount = permissionArray.count();
    mPermissions.reserve(roleArrayCount);
    for (int i = 0; i < roleArrayCount; ++i) {
        Permission r;
        if (r.parsePermission(permissionArray.at(i).toObject(), roleInfo)) {
            if (r.isValid()) {
                mPermissions.append(r);
            } else {
                qCWarning(RUQOLA_LOG) << "Invalid permission: " << permissionArray.at(i).toObject();
            }
        }
    }
}
