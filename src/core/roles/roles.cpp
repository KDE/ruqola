/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roles.h"
#include "ruqola_debug.h"

#include <QJsonArray>

Roles::Roles() = default;

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
    // qDebug() << " type " << type << " id " << id << " userId" << userId;
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
    const auto roleArrayCount = roleArray.count();
    mRoles.reserve(roleArrayCount);
    for (auto i = 0; i < roleArrayCount; ++i) {
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
    for (const Role &r : std::as_const(mRoles)) {
        if (r.userId() == userId) {
            return r;
        }
    }
    return {};
}
