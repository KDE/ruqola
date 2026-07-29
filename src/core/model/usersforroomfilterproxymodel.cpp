/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroomfilterproxymodel.h"

#include "usersforroommodel.h"
#include <QAbstractProxyModel>

using namespace Qt::Literals::StringLiterals;

namespace
{
[[nodiscard]] const UsersForRoomModel *sourceUsersForRoomModel(const QAbstractItemModel *model)
{
    const QAbstractItemModel *currentModel = model;
    while (currentModel) {
        if (const auto *usersModel = qobject_cast<const UsersForRoomModel *>(currentModel)) {
            return usersModel;
        }

        const auto *proxyModel = qobject_cast<const QAbstractProxyModel *>(currentModel);
        if (!proxyModel) {
            return nullptr;
        }
        currentModel = proxyModel->sourceModel();
    }

    return nullptr;
}
}

UsersForRoomFilterProxyModel::UsersForRoomFilterProxyModel(QObject *parent)
    : SortFilterProxyModelBase(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortRole(UsersForRoomModel::UsersForRoomRoles::UserName);
    sort(0);
    setRecursiveFilteringEnabled(true);
}

UsersForRoomFilterProxyModel::~UsersForRoomFilterProxyModel() = default;

void UsersForRoomFilterProxyModel::clearFilter()
{
    mFilterString.clear();
    mStatusType = UsersForRoomFilterProxyModel::FilterUserType::All;
}

bool UsersForRoomFilterProxyModel::hasFullList() const
{
    if (const auto *usersModel = sourceUsersForRoomModel(sourceModel())) {
        return usersModel->hasFullList();
    }
    return false;
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
        beginFilterChange();
        mStatusType = statusType;
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
    }
}

bool UsersForRoomFilterProxyModel::loadMoreUsersInProgress() const
{
    if (const auto *usersModel = sourceUsersForRoomModel(sourceModel())) {
        return usersModel->loadMoreUsersInProgress();
    }
    return false;
}

int UsersForRoomFilterProxyModel::total() const
{
    if (const auto *usersModel = sourceUsersForRoomModel(sourceModel())) {
        return usersModel->total();
    }
    return 0;
}

int UsersForRoomFilterProxyModel::numberOfUsers() const
{
    if (const auto *usersModel = sourceUsersForRoomModel(sourceModel())) {
        return usersModel->usersCount();
    }
    return 0;
}

bool UsersForRoomFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    auto match = [&](int role) {
        return mFilterString.isEmpty() || contains(sourceIndex.data(role).toString());
    };
    switch (mStatusType) {
    case UsersForRoomFilterProxyModel::FilterUserType::All:
        return match(UsersForRoomModel::UsersForRoomRoles::UserName) || match(UsersForRoomModel::UsersForRoomRoles::Name);
    case UsersForRoomFilterProxyModel::FilterUserType::Online:
    case UsersForRoomFilterProxyModel::FilterUserType::Offline:
    case UsersForRoomFilterProxyModel::FilterUserType::Away:
    case UsersForRoomFilterProxyModel::FilterUserType::Busy:
    case UsersForRoomFilterProxyModel::FilterUserType::Owners:
        break;
    }

    if (mStatusType == UsersForRoomFilterProxyModel::FilterUserType::Owners) {
        const QStringList roles = sourceIndex.data(UsersForRoomModel::Roles).toStringList();
        return roles.contains(u"owner"_s) && QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
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

        if (!match(UsersForRoomModel::UsersForRoomRoles::UserName) && !match(UsersForRoomModel::UsersForRoomRoles::Name)) {
            return false;
        }
        return (mStatusType == userStatus);
    }

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

#include "moc_usersforroomfilterproxymodel.cpp"
