/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usercompleterfilterproxymodel.h"
#include "usercompletermodel.h"

UserCompleterFilterProxyModel::UserCompleterFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(UserCompleterModel::UserName);
    sort(0);
}

UserCompleterFilterProxyModel::~UserCompleterFilterProxyModel() = default;

void UserCompleterFilterProxyModel::clear()
{
    static_cast<UserCompleterModel *>(sourceModel())->clear();
}

bool UserCompleterFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, UserCompleterModel::UserName).toString();
        const QString rightString = sourceModel()->data(right, UserCompleterModel::UserName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

#include "moc_usercompleterfilterproxymodel.cpp"
