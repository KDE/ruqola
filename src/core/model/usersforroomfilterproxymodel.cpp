/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

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
    setRecursiveFilteringEnabled(true);
}

UsersForRoomFilterProxyModel::~UsersForRoomFilterProxyModel() = default;

void UsersForRoomFilterProxyModel::clearFilter()
{
    setFilterFixedString({});
    mStatusType = UsersForRoomFilterProxyModel::FilterUserType::All;
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

void UsersForRoomFilterProxyModel::setStatusType(UsersForRoomFilterProxyModel::FilterUserType statusType)
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
    switch (mStatusType) {
    case UsersForRoomFilterProxyModel::FilterUserType::All:
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    case UsersForRoomFilterProxyModel::FilterUserType::Online:
    case UsersForRoomFilterProxyModel::FilterUserType::Offline:
    case UsersForRoomFilterProxyModel::FilterUserType::Away:
    case UsersForRoomFilterProxyModel::FilterUserType::Busy:
    case UsersForRoomFilterProxyModel::FilterUserType::Owners:
        break;
    }

    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    if (mStatusType == UsersForRoomFilterProxyModel::FilterUserType::Owners) {
        const QStringList roles = sourceIndex.data(UsersForRoomModel::Roles).toStringList();
        return roles.contains(QStringLiteral("owner")) && QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    } else {
        UsersForRoomFilterProxyModel::FilterUserType userStatus = UsersForRoomFilterProxyModel::FilterUserType::All;
        const User::PresenceStatus statusType = sourceIndex.data(UsersForRoomModel::Status).value<User::PresenceStatus>();
        switch (statusType) {
        case User::PresenceStatus::Online:
            userStatus = UsersForRoomFilterProxyModel::FilterUserType::Online;
            break;
        case User::PresenceStatus::Busy:
            userStatus = UsersForRoomFilterProxyModel::FilterUserType::Busy;
            break;
        case User::PresenceStatus::Away:
            userStatus = UsersForRoomFilterProxyModel::FilterUserType::Away;
            break;
        case User::PresenceStatus::Offline:
            userStatus = UsersForRoomFilterProxyModel::FilterUserType::Offline;
            break;
        case User::PresenceStatus::Unknown:
            break;
        }
        return (mStatusType == userStatus) && QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

#include "moc_usersforroomfilterproxymodel.cpp"
