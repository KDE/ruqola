/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "permissions.h"
#include "ruqola_debug.h"

#include <QJsonArray>

Permissions::Permissions()
{
}

QVector<Permission> Permissions::permissions() const
{
    return mPermissions;
}

bool Permissions::isEmpty() const
{
    return mPermissions.isEmpty();
}

void Permissions::setPermissions(const QVector<Permission> &roles)
{
    mPermissions = roles;
}

void Permissions::parsePermissions(const QJsonObject &obj)
{
    mPermissions.clear();

    const QJsonArray permissionArray = obj[QStringLiteral("update")].toArray();
    const int roleArrayCount = permissionArray.count();
    mPermissions.reserve(roleArrayCount);
    for (int i = 0; i < roleArrayCount; ++i) {
        Permission r;
        r.parsePermission(permissionArray.at(i).toObject());
        if (r.isValid()) {
            mPermissions.append(r);
        } else {
            qCWarning(RUQOLA_LOG) << "Invalid role" << permissionArray.at(i).toObject();
        }
    }
}
