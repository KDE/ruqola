/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionmanager.h"
#include "ruqola_debug.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
PermissionManager::PermissionManager() = default;

PermissionManager::~PermissionManager() = default;

void PermissionManager::parsePermissions(const QJsonObject &replyObject)
{
    const QJsonArray removeArray = replyObject["remove"_L1].toArray();
    if (!removeArray.isEmpty()) {
        parseRemovePermission(removeArray);
    }
    const QJsonArray updateArray = replyObject["update"_L1].toArray();
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
    return id == QLatin1StringView("mail-messages") || id == "pin-message"_L1 || id == "delete-own-message"_L1 || id == QLatin1StringView("create-invite-links")
        || id == "edit-message"_L1 || id == "delete-message"_L1 || id == QLatin1StringView("edit-room") || id == "clean-channel-history"_L1
        || id == "auto-translate"_L1 || id == QLatin1StringView("snippet-message") || id == "force-delete-message"_L1 || id == "edit-room-avatar"_L1
        || id == QLatin1StringView("create-c") || id == "add-user-to-joined-room"_L1 || id == "leave-c"_L1 || id == QLatin1StringView("create-p")
        || id == "create-d"_L1 || id == "leave-p"_L1 || id == QLatin1StringView("add-user-to-any-c-room") || id == "add-user-to-any-p-room"_L1
        || id == "view-logs"_L1 || id == QLatin1StringView("unarchive-room") || id == "archive-room"_L1 || id == "mute-user"_L1
        || id == QLatin1StringView("remove-user") || id == "view-c-room"_L1 || id == "view-d-room"_L1 || id == QLatin1StringView("view-outside-room")
        || id == "view-statistics"_L1 || id == "post-readonly"_L1 || id == QLatin1StringView("manage-sounds") || id == "set-moderator"_L1
        || id == "set-leader"_L1 || id == QLatin1StringView("set-owner") || id == "delete-user"_L1 || id == "edit-room-retention-policy"_L1
        || id == "access-setting-permissions"_L1 || id == "start-discussion"_L1 || id == QLatin1StringView("start-discussion-other-user")
        || id == "force-delete-message"_L1 || id == "create-team"_L1 || id == QLatin1StringView("convert-team") || id == "delete-team"_L1
        || id == "edit-team"_L1 || id == QLatin1StringView("add-team-member") || id == "edit-team-member"_L1 || id == "add-team-channel"_L1
        || id == QLatin1StringView("edit-team-channel") || id == "remove-team-channel"_L1 || id == "view-all-team-channels"_L1
        || id == QLatin1StringView("view-all-teams") || id == "access-permissions"_L1 || id == "edit-other-user-e2ee"_L1
        || id == QLatin1StringView("edit-other-user-totp") || id == "assign-admin-role"_L1 || id == "edit-other-user-info"_L1
        || id == "edit-other-user-active-status"_L1 || id == "logout-device-management"_L1 || id == "view-device-management"_L1
        || id == "create-personal-access-tokens"_L1 || id == QLatin1StringView("manage-oauth-apps") || id == "create-user"_L1 || id == "bulk-register-user"_L1
        || id == "view-broadcast-member-list"_L1 /* TODO implement view-broadcast-member-list */
        || id == "videoconf-ring-users"_L1 || id == "edit-other-user-password"_L1 || id == QLatin1StringView("view-moderation-console")
        || id == "manage-moderation-actions"_L1 || id == "manage-emoji"_L1 || id == "view-room-administration"_L1 || id == "view-user-administration"_L1;
}

void PermissionManager::parseUpdatePermission(const QJsonArray &updateArray)
{
    for (int i = 0; i < updateArray.count(); ++i) {
        const QJsonObject obj = updateArray.at(i).toObject();
        const QString id = obj["_id"_L1].toString();
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
        if (updateArray.at(0).toString() == "updated"_L1) {
            const QJsonObject obj = updateArray.at(1).toObject();
            const QString id = obj["_id"_L1].toString();
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
