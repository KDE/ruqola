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
    const QJsonArray removeArray = replyObject[QLatin1String("remove")].toArray();
    if (!removeArray.isEmpty()) {
        parseRemovePermission(removeArray);
    }
    const QJsonArray updateArray = replyObject[QLatin1String("update")].toArray();
    parseUpdatePermission(updateArray);
    //qDebug() << "mMapPermissions  " << mMapPermissions;
}

bool PermissionManager::contains(const QString &permissionId) const
{
    return mMapPermissions.contains(permissionId);
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
    qCWarning(RUQOLA_LOG) << " void PermissionManager::parseRemovePermission(const QJsonArray &removeArray) not implemented yet" << removeArray;
}

void PermissionManager::parseUpdatePermission(const QJsonArray &updateArray)
{
    for (int i = 0; i < updateArray.count(); ++i) {
        const QJsonObject obj = updateArray.at(i).toObject();
        const QString id = obj[QLatin1String("_id")].toString();
        if (id == QLatin1String("mail-messages")
            || id == QLatin1String("pin-message")
            || id == QLatin1String("delete-own-message")
            || id == QLatin1String("create-invite-links")
            || id == QLatin1String("edit-message")
            || id == QLatin1String("delete-message")
            || id == QLatin1String("edit-room")
            || id == QLatin1String("clean-channel-history")
            || id == QLatin1String("auto-translate")
            || id == QLatin1String("snippet-message")
            || id == QLatin1String("force-delete-message")
            || id == QLatin1String("edit-room-avatar")
            || id == QLatin1String("create-c")
            || id == QLatin1String("add-user-to-joined-room")
            || id == QLatin1String("leave-c")
            || id == QLatin1String("create-p")
            || id == QLatin1String("create-d")
            || id == QLatin1String("leave-p")
            || id == QLatin1String("add-user-to-any-c-room")
            || id == QLatin1String("add-user-to-any-p-room")
            || id == QLatin1String("view-logs")
            || id == QLatin1String("unarchive-room")
            || id == QLatin1String("archive-room")
            || id == QLatin1String("mute-user")
            || id == QLatin1String("remove-user")
            || id == QLatin1String("view-c-room")

            ) {
            Permission p;
            p.parsePermission(obj);
            if (p.isValid()) {
                mMapPermissions.insert(id, p);
            }
        } else {
            qCDebug(RUQOLA_LOG) << "Permission id not stored: " << id;
        }
    }
    //qDebug() << "mMapPermissions " << mMapPermissions;
}
