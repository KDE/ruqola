/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminrolesmodel.h"
#include <KLocalizedString>

AdminRolesModel::AdminRolesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminRolesModel::~AdminRolesModel() = default;

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
            return i18n("Identifier");
        case AdminRolesModel::Name:
            return i18n("Name");
        case AdminRolesModel::Scope:
            return i18n("Scope");
        case AdminRolesModel::Protected:
            return i18n("Protected");
        case AdminRolesModel::Mandatory2Fa:
            return i18n("Two Factors Authentication");
        case AdminRolesModel::Description:
            return i18n("Description");
        }
    }
    return {};
}

int AdminRolesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(AdminRolesModel::LastColumn) + 1;
    return val;
}

QVector<RoleInfo> AdminRolesModel::roles() const
{
    return mListRoleInfos;
}

void AdminRolesModel::clear()
{
    if (!mListRoleInfos.isEmpty()) {
        beginResetModel();
        mListRoleInfos.clear();
        endResetModel();
    }
}

void AdminRolesModel::setRoles(const QVector<RoleInfo> &newRoles)
{
    clear();
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
    case AdminRolesModel::Protected:
        return roleInfo.roleProtected();
    case AdminRolesModel::Mandatory2Fa:
        return roleInfo.mandatory2fa();
    case AdminRolesModel::Description:
        return roleInfo.description();
    }
    return {};
}

#include "moc_adminrolesmodel.cpp"
