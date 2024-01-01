/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroomfilterproxymodel.h"
#include "usersforroommodel.h"

UsersForRoomFilterProxyModel::UsersForRoomFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    // Filter on alias/username ?
    setFilterRole(UsersForRoomModel::UsersForRoomRoles::UserName);
    setSortRole(UsersForRoomModel::UsersForRoomRoles::UserName);
    sort(0);
}

UsersForRoomFilterProxyModel::~UsersForRoomFilterProxyModel() = default;

void UsersForRoomFilterProxyModel::clearFilter()
{
    setFilterFixedString({});
    mStatusType.clear();
}

void UsersForRoomFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

bool UsersForRoomFilterProxyModel::hasFullList() const
{
    return static_cast<UsersForRoomModel *>(sourceModel())->hasFullList();
}

bool UsersForRoomFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, UsersForRoomModel::UserName).toString();
        const QString rightString = sourceModel()->data(right, UsersForRoomModel::UserName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

void UsersForRoomFilterProxyModel::setStatusType(const QString &statusType)
{
    if (mStatusType != statusType) {
        mStatusType = statusType;
        invalidateFilter();
    }
}

bool UsersForRoomFilterProxyModel::loadMoreUsersInProgress() const
{
    return static_cast<UsersForRoomModel *>(sourceModel())->loadMoreUsersInProgress();
}

int UsersForRoomFilterProxyModel::total() const
{
    return static_cast<UsersForRoomModel *>(sourceModel())->total();
}

int UsersForRoomFilterProxyModel::numberOfUsers() const
{
    return static_cast<UsersForRoomModel *>(sourceModel())->usersCount();
}

bool UsersForRoomFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mStatusType.isEmpty()) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString statusType = sourceIndex.data(UsersForRoomModel::Status).toString();
    return (mStatusType == statusType) && QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

#include "moc_usersforroomfilterproxymodel.cpp"
