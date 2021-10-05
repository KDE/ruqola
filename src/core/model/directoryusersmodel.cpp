/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "directoryusersmodel.h"
#include "discussions.h"
#include <KLocalizedString>

DirectoryUsersModel::DirectoryUsersModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

DirectoryUsersModel::~DirectoryUsersModel()
{
}

void DirectoryUsersModel::checkFullList()
{
    setHasFullList(mUsers.count() == mUsers.total());
}

int DirectoryUsersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mUsers.count();
}

QList<int> DirectoryUsersModel::hideColumns() const
{
    return {UserId, JoinAtDateTime};
}

Users::ParseType DirectoryUsersModel::parseType() const
{
    return Users::ParseType::Directory;
}

void DirectoryUsersModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mUsers.count();
    mUsers.parseMoreUsers(obj, parseType());
    beginInsertRows(QModelIndex(), numberOfElement, mUsers.count() - 1);
    endInsertRows();
    checkFullList();
}

void DirectoryUsersModel::parseElements(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mUsers.count() - 1);
        mUsers.clear();
        endRemoveRows();
    }
    mUsers.parseUsers(obj, parseType());
    if (!mUsers.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mUsers.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

QVariant DirectoryUsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }
    const User &user = mUsers.at(index.row());
    const int col = index.column();
    switch (static_cast<DirectoryUsersRoles>(col)) {
    case DirectoryUsersRoles::Name:
        return user.name().isEmpty() ? user.userName() : user.name();
    case DirectoryUsersRoles::Email:
        return user.userEmailsInfo().email;
    case DirectoryUsersRoles::JoinAt:
        return user.createdAt().toString(); // TODO verify it
    case DirectoryUsersRoles::JoinAtDateTime:
        return user.createdAt();
    case DirectoryUsersRoles::UserId:
        return user.userId();
    }
    return {};
}

QVariant DirectoryUsersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<DirectoryUsersRoles>(section)) {
        case DirectoryUsersModel::Name:
            return i18n("Name");
        case DirectoryUsersModel::Email:
            return i18n("Emails");
        case DirectoryUsersModel::JoinAt:
            return i18n("Join At");
        case DirectoryUsersModel::JoinAtDateTime:
        case DirectoryUsersModel::UserId:
            return {};
        }
    }
    return QVariant();
}

int DirectoryUsersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(DirectoryUsersModel::LastColumn) + 1;
}

int DirectoryUsersModel::total() const
{
    return mUsers.total();
}
