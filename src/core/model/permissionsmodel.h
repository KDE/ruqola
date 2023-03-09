/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "permissions/permissions.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT PermissionsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AdminPermissionsRoles {
        Identifier,
        Roles,
        RolesStr,
        LastColumn = RolesStr,
    };
    Q_ENUM(AdminPermissionsRoles)

    explicit PermissionsModel(QObject *parent = nullptr);
    ~PermissionsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT Permissions permissions() const;
    void setPermissions(const Permissions &newPermissions);

private:
    Q_DISABLE_COPY(PermissionsModel)
    Permissions mPermissions;
};
