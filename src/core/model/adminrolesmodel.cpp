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

#include "adminrolesmodel.h"
#include <KLocalizedString>

AdminRolesModel::AdminRolesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminRolesModel::~AdminRolesModel()
{
}

int AdminRolesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mListRoleInfos.count();
}

QVariant AdminRolesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminRoles>(section)) {
        case AdminRolesModel::Identifier:
            return i18n("Name");
        case AdminRolesModel::Name:
            return i18n("Name");
        case AdminRolesModel::Scope:
            return i18n("Scope");
        }
    }
    return QVariant();
}

int AdminRolesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminRolesModel::LastColumn) + 1;
}

QVector<RoleInfo> AdminRolesModel::roles() const
{
    return mListRoleInfos;
}

void AdminRolesModel::setRoles(const QVector<RoleInfo> &newRoles)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mListRoleInfos.count() - 1);
        mListRoleInfos.clear();
        endRemoveRows();
    }
    if (!newRoles.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newRoles.count() - 1);
        mListRoleInfos = newRoles;
        endInsertRows();
    }
}

QVariant AdminRolesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mListRoleInfos.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const RoleInfo &roleInfo = mListRoleInfos.at(index.row());
    const int col = index.column();
    switch (col) {
    case AdminRolesModel::Identifier:
        return roleInfo.identifier();
    case AdminRolesModel::Name:
        return roleInfo.name();
    case AdminRolesModel::Scope:
        return roleInfo.scope();
    }
    return {};
}
