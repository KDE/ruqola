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

#include "rolesmanager.h"
#include "ruqola_debug.h"

#include <QJsonArray>
RolesManager::RolesManager(QObject *parent)
    : QObject{parent}
{
}

RolesManager::~RolesManager()
{
}

void RolesManager::parseRoles(const QJsonObject &obj)
{
    const QJsonArray array = obj[QLatin1String("roles")].toArray();

    mRoleInfo.reserve(array.count());
    for (const QJsonValue &current : array) {
        const QJsonObject roleObject = current.toObject();
        RoleInfo info;
        info.parseRoleInfo(roleObject);
        mRoleInfo.append(info);
    }
}

void RolesManager::updateRoles(const QJsonArray &contents)
{
    for (const QJsonValue &current : contents) {
        const QJsonObject roleObject = current.toObject();
        const QString type = roleObject.value(QStringLiteral("type")).toString();
        if (type == QLatin1String("removed")) {
            // TODO
        } else if (type == QLatin1String("changed")) {
            // TODO
        }
    }
    // QJsonObject({"args":[{"_id":"vFXCWG9trXLti6xQm","name":"vFXCWG9trXLti6xQm","type":"removed"}],"eventName":"roles"})
    // QJsonObject({"args":[{"_id":"hiafuM2enNapgD2mg","_updatedAt":{"$date":1634588706596},"description":"","mandatory2fa":false,"name":"test4","protected":false,"scope":"Users","type":"changed"}],"eventName":"roles"})

    // TODO
}

const QList<RoleInfo> &RolesManager::roleInfo() const
{
    return mRoleInfo;
}
