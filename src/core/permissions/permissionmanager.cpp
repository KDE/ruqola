/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionmanager.h"
#include "ruqola_debug.h"
#include <QDebug>
#include <QJsonArray>
PermissionManager::PermissionManager() = default;

PermissionManager::~PermissionManager() = default;

void PermissionManager::parsePermissions(const QJsonObject &replyObject)
{
    const QJsonArray removeArray = replyObject[QLatin1String("remove")].toArray();
    if (!removeArray.isEmpty()) {
        parseRemovePermission(removeArray);
    }
    const QJsonArray updateArray = replyObject[QLatin1String("update")].toArray();
    parseUpdatePermission(updateArray);
    // qDebug() << "mMapPermissions  " << mMapPermissions;
}

const Permission PermissionManager::permission(const QString &permissionId) const
{
    return mMapPermissions.value(permissionId);
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

bool PermissionManager::storePermission(const QString &id)
{
    return id == QLatin1String("mail-messages") || id == QLatin1String("pin-message") || id == QLatin1String("delete-own-message")
        || id == QLatin1String("create-invite-links") || id == QLatin1String("edit-message") || id == QLatin1String("delete-message")
        || id == QLatin1String("edit-room") || id == QLatin1String("clean-channel-history") || id == QLatin1String("auto-translate")
        || id == QLatin1String("snippet-message") || id == QLatin1String("force-delete-message") || id == QLatin1String("edit-room-avatar")
        || id == QLatin1String("create-c") || id == QLatin1String("add-user-to-joined-room") || id == QLatin1String("leave-c")
        || id == QLatin1String("create-p") || id == QLatin1String("create-d") || id == QLatin1String("leave-p") || id == QLatin1String("add-user-to-any-c-room")
        || id == QLatin1String("add-user-to-any-p-room") || id == QLatin1String("view-logs") || id == QLatin1String("unarchive-room")
        || id == QLatin1String("archive-room") || id == QLatin1String("mute-user") || id == QLatin1String("remove-user") || id == QLatin1String("view-c-room")
        || id == QLatin1String("view-statistics") || id == QLatin1String("post-readonly") || id == QLatin1String("manage-sounds")
        || id == QLatin1String("set-moderator") || id == QLatin1String("set-leader") || id == QLatin1String("set-owner") || id == QLatin1String("delete-user")
        || id == QLatin1String("edit-room-retention-policy") || id == QLatin1String("access-setting-permissions") || id == QLatin1String("start-discussion")
        || id == QLatin1String("start-discussion-other-user") || id == QLatin1String("force-delete-message") || id == QLatin1String("create-team")
        || id == QLatin1String("convert-team") || id == QLatin1String("delete-team") || id == QLatin1String("edit-team")
        || id == QLatin1String("add-team-member") || id == QLatin1String("edit-team-member") || id == QLatin1String("add-team-channel")
        || id == QLatin1String("edit-team-channel") || id == QLatin1String("remove-team-channel") || id == QLatin1String("view-all-team-channels")
        || id == QLatin1String("view-all-teams") || id == QLatin1String("access-permissions") || id == QLatin1String("edit-other-user-e2ee")
        || id == QLatin1String("edit-other-user-totp") || id == QLatin1String("assign-admin-role") || id == QLatin1String("edit-other-user-info")
        || id == QLatin1String("edit-other-user-active-status") || id == QLatin1String("logout-device-management")
        || id == QLatin1String("view-device-management") || id == QLatin1String("create-personal-access-tokens") || id == QLatin1String("manage-oauth-apps")
        || id == QLatin1String("create-user");
}

void PermissionManager::parseUpdatePermission(const QJsonArray &updateArray)
{
    for (int i = 0; i < updateArray.count(); ++i) {
        const QJsonObject obj = updateArray.at(i).toObject();
        const QString id = obj[QLatin1String("_id")].toString();
        if (storePermission(id)) {
            Permission p;
            p.parsePermission(obj /* add roles? */);
            // qDebug() << "obj " << obj;
            if (p.isValid()) {
                mMapPermissions.insert(id, p);
            }
        } else {
            qCDebug(RUQOLA_LOG) << "Permission id not stored: " << id;
        }
    }
    // qDebug() << "mMapPermissions " << mMapPermissions;
}

bool PermissionManager::updatePermission(const QJsonArray &updateArray)
{
    bool updatedPermission = false;
    if (updateArray.count() == 2) {
        if (updateArray.at(0).toString() == QLatin1String("updated")) {
            const QJsonObject obj = updateArray.at(1).toObject();
            const QString id = obj[QLatin1String("_id")].toString();
            if (storePermission(id)) {
                Permission p;
                p.parsePermission(obj, {} /* add roles? */, false); // We use date from ddpclient not restapi
                if (p.isValid()) {
                    mMapPermissions.insert(id, p);
                    updatedPermission = true;
                }
            }
        }
    } else {
        qCWarning(RUQOLA_LOG) << " PermissionManager::updatePermission invalid updateArray count " << updateArray.count();
    }
    return updatedPermission;
    // QJsonObject({"args":["updated",{"_id":"access-mailer","_updatedAt":{"$date":1634569746270},"roles":["admin","vFXCWG9trXLti6xQm"]}],"eventName":"permissions-changed"}
}
