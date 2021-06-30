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

#include "rolesmodel.h"

RolesModel::RolesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

RolesModel::~RolesModel()
{
}

int RolesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mRoles.count();
}

QVariant RolesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoles.count()) {
        return {};
    }
    const RoleInfo &roleInfo = mRoles.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case RolesInfoRoles::Name: {
        const QString name = roleInfo.name();
        return name;
    }
    case RolesInfoRoles::Identifier:
        return roleInfo.identifier();
    case Qt::CheckStateRole: {
        const QString roomId = data(index, RolesInfoRoles::Identifier).toString();
        return mRolesSelected.contains(roomId) ? Qt::Checked : Qt::Unchecked;
    }
    }
    return {};
}

void RolesModel::setRoles(const QVector<RoleInfo> &newRoles)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mRoles.count() - 1);
        mRoles.clear();
        endRemoveRows();
    }
    if (!newRoles.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newRoles.count() - 1);
        mRoles = newRoles;
        endInsertRows();
    }
}

const QVector<RoleInfo> &RolesModel::roles() const
{
    return mRoles;
}

const QStringList &RolesModel::rolesSelected() const
{
    return mRolesSelected;
}

void RolesModel::setRolesSelected(const QStringList &newRolesSelected)
{
    mRolesSelected = newRolesSelected;
}

bool RolesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        if (index.isValid()) {
            Q_EMIT dataChanged(index, index);
            const QString roleId = data(index, RolesModel::Identifier).toString();
            if (value == Qt::Checked) {
                mRolesSelected.append(roleId);
            } else {
                mRolesSelected.removeAll(roleId);
            }
            return true;
        }
    }
    return QAbstractListModel::setData(index, value, role);
}

Qt::ItemFlags RolesModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
    } else {
        return QAbstractListModel::flags(index);
    }
}
