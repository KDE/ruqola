/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryusersmodel.h"
#include <KLocalizedString>

DirectoryUsersModel::DirectoryUsersModel(QObject *parent)
    : CustomBaseModel(parent)
{
}

DirectoryUsersModel::~DirectoryUsersModel() = default;

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
    return {UserId, JoinAtDateTime, UserName};
}

Users::ParseType DirectoryUsersModel::parseType() const
{
    return Users::ParseType::Directory;
}

void DirectoryUsersModel::setRoles(const QVector<RoleInfo> &newRoles)
{
    mRoleInfo = newRoles;
}

void DirectoryUsersModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mUsers.count();
    mUsers.parseMoreUsers(obj, parseType(), mRoleInfo);
    beginInsertRows(QModelIndex(), numberOfElement, mUsers.count() - 1);
    endInsertRows();
    checkFullList();
}

void DirectoryUsersModel::parseElements(const QJsonObject &obj)
{
    if (!mUsers.isEmpty()) {
        beginResetModel();
        mUsers.clear();
        endResetModel();
    }
    mUsers.parseUsers(obj, parseType(), mRoleInfo);
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
    case DirectoryUsersModel::UserName:
        return user.userName();
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
        case DirectoryUsersModel::UserName:
            return {};
        }
    }
    return {};
}

int DirectoryUsersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(DirectoryUsersModel::LastColumn) + 1;
    return val;
}

int DirectoryUsersModel::total() const
{
    return mUsers.total();
}

#include "moc_directoryusersmodel.cpp"
