/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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


#include "rolesforroommodel.h"

RolesForRoomModel::RolesForRoomModel(QObject *parent)
    : QAbstractListModel (parent)
{

}

RolesForRoomModel::~RolesForRoomModel()
{

}

void RolesForRoomModel::setRoles(const QVector<Role> &roles)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mRoles.count() - 1);
        mRoles.clear();
        endRemoveRows();
    }
    if (!mRoles.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mRoles.count() - 1);
        mRoles = roles;
        endInsertRows();
    }
}

int RolesForRoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mRoles.count();
}

QVariant RolesForRoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoles.count()) {
        return QVariant();
    }
    //TODO
    return {};
}

QHash<int, QByteArray> RolesForRoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    //TODO
    return roles;
}
