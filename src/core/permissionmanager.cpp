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

#include "permissionmanager.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QDebug>
PermissionManager::PermissionManager()
{
}

PermissionManager::~PermissionManager()
{
}

void PermissionManager::parsePermissions(const QJsonObject &replyObject)
{
    //TODO implement remove/update
    const QJsonArray removeArray = replyObject[QLatin1String("remove")].toArray();
    parseRemovePermission(removeArray);
    const QJsonArray updateArray = replyObject[QLatin1String("update")].toArray();
    parseUpdatePermission(updateArray);
    //TODO
    qDebug() << "mMapPermissions  " << mMapPermissions;
}

QStringList PermissionManager::roles(const QString &permissionId) const
{
    const Permission p = mMapPermissions.value(permissionId);
    if (p.isValid()) {
        return p.roles();
    }
    return {};
}

int PermissionManager::permissionCount() const
{
    return mMapPermissions.count();
}

void PermissionManager::parseRemovePermission(const QJsonArray &removeArray)
{
    qDebug() << " void PermissionManager::parseRemovePermission(const QJsonArray &removeArray) not implemented yet";
}

void PermissionManager::parseUpdatePermission(const QJsonArray &updateArray)
{
    for (int i = 0; i < updateArray.count(); ++i) {
        const QJsonObject obj = updateArray.at(i).toObject();
        const QString id = obj[QLatin1String("_id")].toString();
        if (id == QLatin1String("mail-messages")
            || id == QLatin1String("pin-message")
            || id == QLatin1String("delete-own-message")
            || id == QLatin1String("create-invite-links")) {
            Permission p;
            p.parsePermission(obj);
            if (p.isValid()) {
                mMapPermissions.insert(id, p);
            }
        } else {
            qDebug() << " id " << id;
        }
    }
}
