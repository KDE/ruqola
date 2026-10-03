/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "permission.h"
#include <QHash>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT PermissionsManager
{
public:
    PermissionsManager();

    void parsePermissions(const QJsonObject &replyObject);

    [[nodiscard]] QStringList roles(QStringView permissionId) const;
    [[nodiscard]] int permissionCount() const;
    [[nodiscard]] bool contains(QStringView permissionId) const;

    [[nodiscard]] bool updatePermission(const QJsonArray &updateArray);
    void parseUpdatePermission(const QJsonArray &removeArray);

    [[nodiscard]] const Permission permission(QStringView permissionId) const;

    // Only for debugging permissions. (debug mode)
    [[nodiscard]] QList<Permission> permissions() const;

private:
    [[nodiscard]] static bool storePermission(const QString &id);
    static void parseRemovePermission(const QJsonArray &removeArray);
    QHash<QString, Permission> mMapPermissions;
};
