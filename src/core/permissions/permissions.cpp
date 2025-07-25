/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissions.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

Permissions::Permissions() = default;

QList<Permission> Permissions::permissions() const
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
    if (index < 0 || index >= mPermissions.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mPermissions.at(index);
}

bool Permissions::isEmpty() const
{
    return mPermissions.isEmpty();
}

void Permissions::setPermissions(const QList<Permission> &roles)
{
    mPermissions = roles;
}

void Permissions::parsePermissions(const QJsonObject &obj, const QString &str, const QList<RoleInfo> &roleInfo)
{
    mPermissions.clear();

    const QJsonArray permissionArray = obj[str.isEmpty() ? u"update"_s : str].toArray();
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
