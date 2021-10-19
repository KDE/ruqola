/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

    void updatePermission(const QJsonArray &updateArray);
    void parseUpdatePermission(const QJsonArray &removeArray);

private:
    Q_REQUIRED_RESULT bool storePermission(const QString &id);
    void parseRemovePermission(const QJsonArray &removeArray);
    QMap<QString, Permission> mMapPermissions;
};
