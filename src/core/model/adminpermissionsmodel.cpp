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
    return mAdminInvites.count();
}

QVariant AdminPermissionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminPermissionsRoles>(section)) {
        case AdminPermissionsModel::Name:
            return i18n("Name");
        case AdminPermissionsModel::Permissions:
            return i18n("Permissions");
        }
    }
    return QVariant();
}

int AdminPermissionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminPermissionsModel::LastColumn) + 1;
}

const QVector<InviteInfo> &AdminPermissionsModel::adminInvites() const
{
    return mAdminInvites;
}

void AdminPermissionsModel::setAdminInvites(const QVector<InviteInfo> &newAdminInvites)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mAdminInvites.count() - 1);
        mAdminInvites.clear();
        endRemoveRows();
    }
    if (!newAdminInvites.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newAdminInvites.count() - 1);
        mAdminInvites = newAdminInvites;
        endInsertRows();
    }
}

QVariant AdminPermissionsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAdminInvites.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const InviteInfo &inviteInfo = mAdminInvites.at(index.row());
    const int col = index.column();
    switch (col) {
    case AdminPermissionsModel::Name:
        return inviteInfo.userIdentifier();
    case AdminPermissionsModel::Permissions:
        return inviteInfo.identifier();
    }
    return {};
}

