/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "roles.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

Roles::Roles()
{
}

QVector<Role> Roles::roles() const
{
    return mRoles;
}

bool Roles::isEmpty() const
{
    return mRoles.isEmpty();
}

void Roles::setRoles(const QVector<Role> &roles)
{
    mRoles = roles;
}

void Roles::updateRoles(const QJsonObject &obj)
{
    const QString type = obj[QLatin1String("type")].toString();
    const QString id = obj[QLatin1String("_id")].toString();
    const QString userId = obj[QLatin1String("u")].toObject().value(QLatin1String("_id")).toString();
    bool foundUser = false;
    //qDebug() << " type " << type << " id " << id << " userId" << userId;
    if (type == QLatin1String("added")) {
        for (int i = 0, total = mRoles.count(); i < total; ++i) {
            if (mRoles.at(i).userId() == userId) {
                Role &r = mRoles[i];
                r.updateRole(id, true);
                foundUser = true;
                break;
            }
        }
        if (!foundUser) {
            Role r;
            r.setUserId(userId);
            r.updateRole(id, true);
            mRoles.append(r);
        }
    } else if (type == QLatin1String("removed")) {
        for (int i = 0, total = mRoles.count(); i < total; ++i) {
            if (mRoles.at(i).userId() == userId) {
                Role r = mRoles.takeAt(i);
                r.updateRole(id, false);
                if (r.hasARole()) {
                    mRoles.append(r);
                }
                foundUser = true;
                break;
            }
        }
        if (!foundUser) {
            qCWarning(RUQOLA_LOG) << "Problem you want to remove role for an not existing role! it seems to be a bug ";
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown change role type " << type;
    }
}

void Roles::parseRole(const QJsonObject &obj)
{
    mRoles.clear();

    const QJsonArray roleArray = obj[QStringLiteral("roles")].toArray();
    const int roleArrayCount = roleArray.count();
    mRoles.reserve(roleArrayCount);
    for (int i = 0; i < roleArrayCount; ++i) {
        Role r;
        r.parseRole(roleArray.at(i).toObject());
        if (r.isValid()) {
            mRoles.append(r);
        } else {
            qCWarning(RUQOLA_LOG) << "Invalid role" << roleArray.at(i).toObject();
        }
    }
}

Role Roles::findRoleByUserId(const QString &userId) const
{
    for (const Role &r : qAsConst(mRoles)) {
        if (r.userId() == userId) {
            return r;
        }
    }
    return {};
}
