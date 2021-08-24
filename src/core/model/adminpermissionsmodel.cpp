/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "adminpermissionsmodel.h"
#include "permissions.h"
#include <KLocalizedString>
#include <QDateTime>

AdminPermissionsModel::AdminPermissionsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminPermissionsModel::~AdminPermissionsModel()
{
}

int AdminPermissionsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mPermissions.count();
}

QVariant AdminPermissionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminPermissionsRoles>(section)) {
        case AdminPermissionsModel::Identifier:
            return i18n("Name");
        case AdminPermissionsModel::Roles:
            return i18n("Roles");
        }
    }
    return QVariant();
}

int AdminPermissionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminPermissionsModel::LastColumn) + 1;
}

Permissions AdminPermissionsModel::permissions() const
{
    return mPermissions;
}

void AdminPermissionsModel::setPermissions(Permissions newPermissions)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mPermissions.count() - 1);
        mPermissions.clear();
        endRemoveRows();
    }
    if (!newPermissions.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newPermissions.count() - 1);
        mPermissions = newPermissions;
        endInsertRows();
    }
}

QVariant AdminPermissionsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mPermissions.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const Permission &permissionInfo = mPermissions.at(index.row());
    const int col = index.column();
    switch (col) {
    case AdminPermissionsModel::Identifier:
        return permissionInfo.identifier();
    case AdminPermissionsModel::Roles:
        return permissionInfo.roles();
    }
    return {};
}

