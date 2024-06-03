/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "permissions/permissions.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AppsCategoriesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AdminPermissionsRoles {
        IdentifierRole,
        RolesRole,
        RolesStrRole,
        LastColumn = RolesStrRole,
    };
    Q_ENUM(AdminPermissionsRoles)

    explicit AppsCategoriesModel(QObject *parent = nullptr);
    ~AppsCategoriesModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] Permissions permissions() const;
    void setPermissions(const Permissions &newPermissions);

private:
    Permissions mPermissions;
};
