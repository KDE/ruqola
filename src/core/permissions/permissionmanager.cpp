/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionmanager.h"
#include "ruqola_debug.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
PermissionManager::PermissionManager() = default;

PermissionManager::~PermissionManager() = default;

void PermissionManager::parsePermissions(const QJsonObject &replyObject)
{
    const QJsonArray removeArray = replyObject[QLatin1StringView("remove")].toArray();
    if (!removeArray.isEmpty()) {
        parseRemovePermission(removeArray);
    }
    const QJsonArray updateArray = replyObject[QLatin1StringView("update")].toArray();
    parseUpdatePermission(updateArray);
    // qDebug() << "mMapPermissions  " << mMapPermissions;
}

const Permission PermissionManager::permission(const QString &permissionId) const
{
    return mMapPermissions.value(permissionId);
}

QList<Permission> PermissionManager::permissions() const
{
    return mMapPermissions.values().toVector();
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
    return id == QLatin1StringView("mail-messages") || id == QLatin1StringView("pin-message") || id == QLatin1StringView("delete-own-message")
        || id == QLatin1StringView("create-invite-links") || id == QLatin1StringView("edit-message") || id == QLatin1StringView("delete-message")
        || id == QLatin1StringView("edit-room") || id == QLatin1StringView("clean-channel-history") || id == QLatin1StringView("auto-translate")
        || id == QLatin1StringView("snippet-message") || id == QLatin1StringView("force-delete-message") || id == QLatin1StringView("edit-room-avatar")
        || id == QLatin1StringView("create-c") || id == QLatin1StringView("add-user-to-joined-room") || id == QLatin1StringView("leave-c")
        || id == QLatin1StringView("create-p") || id == QLatin1StringView("create-d") || id == QLatin1StringView("leave-p")
        || id == QLatin1StringView("add-user-to-any-c-room") || id == QLatin1StringView("add-user-to-any-p-room") || id == QLatin1StringView("view-logs")
        || id == QLatin1StringView("unarchive-room") || id == QLatin1StringView("archive-room") || id == QLatin1StringView("mute-user")
        || id == QLatin1StringView("remove-user") || id == QLatin1StringView("view-c-room") || id == QLatin1StringView("view-d-room")
        || id == QLatin1StringView("view-outside-room") || id == QLatin1StringView("view-statistics") || id == QLatin1StringView("post-readonly")
        || id == QLatin1StringView("manage-sounds") || id == QLatin1StringView("set-moderator") || id == QLatin1StringView("set-leader")
        || id == QLatin1StringView("set-owner") || id == QLatin1StringView("delete-user") || id == QLatin1StringView("edit-room-retention-policy")
        || id == QLatin1StringView("access-setting-permissions") || id == QLatin1StringView("start-discussion")
        || id == QLatin1StringView("start-discussion-other-user") || id == QLatin1StringView("force-delete-message") || id == QLatin1StringView("create-team")
        || id == QLatin1StringView("convert-team") || id == QLatin1StringView("delete-team") || id == QLatin1StringView("edit-team")
        || id == QLatin1StringView("add-team-member") || id == QLatin1StringView("edit-team-member") || id == QLatin1StringView("add-team-channel")
        || id == QLatin1StringView("edit-team-channel") || id == QLatin1StringView("remove-team-channel") || id == QLatin1StringView("view-all-team-channels")
        || id == QLatin1StringView("view-all-teams") || id == QLatin1StringView("access-permissions") || id == QLatin1StringView("edit-other-user-e2ee")
        || id == QLatin1StringView("edit-other-user-totp") || id == QLatin1StringView("assign-admin-role") || id == QLatin1StringView("edit-other-user-info")
        || id == QLatin1StringView("edit-other-user-active-status") || id == QLatin1StringView("logout-device-management")
        || id == QLatin1StringView("view-device-management") || id == QLatin1StringView("create-personal-access-tokens")
        || id == QLatin1StringView("manage-oauth-apps") || id == QLatin1StringView("create-user") || id == QLatin1StringView("bulk-register-user")
        || id == QLatin1StringView("view-broadcast-member-list") /* TODO implement view-broadcast-member-list */
        || id == QLatin1StringView("videoconf-ring-users") || id == QLatin1StringView("edit-other-user-password")
        || id == QLatin1StringView("view-moderation-console") || id == QLatin1StringView("manage-moderation-actions") || id == QLatin1StringView("manage-emoji")
        || id == QLatin1StringView("view-room-administration") || id == QLatin1StringView("view-user-administration");
}

void PermissionManager::parseUpdatePermission(const QJsonArray &updateArray)
{
    for (int i = 0; i < updateArray.count(); ++i) {
        const QJsonObject obj = updateArray.at(i).toObject();
        const QString id = obj[QLatin1StringView("_id")].toString();
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
        if (updateArray.at(0).toString() == QLatin1StringView("updated")) {
            const QJsonObject obj = updateArray.at(1).toObject();
            const QString id = obj[QLatin1StringView("_id")].toString();
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
