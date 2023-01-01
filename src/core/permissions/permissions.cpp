/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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
    if (index < 0 || index > mPermissions.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
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
    const auto roleArrayCount = permissionArray.count();
    mPermissions.reserve(roleArrayCount);
    for (auto i = 0; i < roleArrayCount; ++i) {
        Permission r;
        if (r.parsePermission(permissionArray.at(i).toObject(), roleInfo)) {
            if (r.isValid()) {
                mPermissions.append(std::move(r));
            } else {
                qCWarning(RUQOLA_LOG) << "Invalid permission: " << permissionArray.at(i).toObject();
            }
        }
    }
}
