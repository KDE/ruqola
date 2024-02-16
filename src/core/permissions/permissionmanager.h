/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "permission.h"
#include <QMap>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT PermissionManager
{
public:
    PermissionManager();
    ~PermissionManager();

    void parsePermissions(const QJsonObject &replyObject);

    [[nodiscard]] QStringList roles(const QString &permissionId) const;
    [[nodiscard]] int permissionCount() const;
    [[nodiscard]] bool contains(const QString &permissionId) const;

    [[nodiscard]] bool updatePermission(const QJsonArray &updateArray);
    void parseUpdatePermission(const QJsonArray &removeArray);

    [[nodiscard]] const Permission permission(const QString &permissionId) const;

    // Only for debugging permissions. (debug mode)
    [[nodiscard]] QList<Permission> permissions() const;

private:
    [[nodiscard]] bool storePermission(const QString &id);
    void parseRemovePermission(const QJsonArray &removeArray);
    QMap<QString, Permission> mMapPermissions;
};
