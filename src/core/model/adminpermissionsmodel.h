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

#include "libruqolacore_export.h"
#include "permissions.h"
#include "roleinfo.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AdminPermissionsModel : public QAbstractListModel
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

    explicit AdminPermissionsModel(QObject *parent = nullptr);
    ~AdminPermissionsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT Permissions permissions() const;
    void setPermissions(const Permissions &newPermissions);

    void setListRoleInfos(const QVector<RoleInfo> &newListRoleInfos);

private:
    Q_DISABLE_COPY(AdminPermissionsModel)
    Permissions mPermissions;
    QVector<RoleInfo> mListRoleInfos;
};
