/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "permission.h"
#include <QJsonObject>
#include <QMap>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT PermissionManager
{
public:
    PermissionManager();
    ~PermissionManager();

    void parsePermissions(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT QStringList roles(const QString &permissionId) const;
    Q_REQUIRED_RESULT int permissionCount() const;
    Q_REQUIRED_RESULT bool contains(const QString &permissionId) const;

    Q_REQUIRED_RESULT bool updatePermission(const QJsonArray &updateArray);
    void parseUpdatePermission(const QJsonArray &removeArray);

    Q_REQUIRED_RESULT const Permission permission(const QString &permissionId) const;

    // Only for debugging permissions. (debug mode)
    Q_REQUIRED_RESULT QVector<Permission> permissions() const;

private:
    Q_REQUIRED_RESULT bool storePermission(const QString &id);
    void parseRemovePermission(const QJsonArray &removeArray);
    QMap<QString, Permission> mMapPermissions;
};
