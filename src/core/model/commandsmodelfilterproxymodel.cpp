/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandsmodelfilterproxymodel.h"
#include "commandsmodel.h"

CommandsModelFilterProxyModel::CommandsModelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(CommandsModel::CommandName);
    sort(0);
}

CommandsModelFilterProxyModel::~CommandsModelFilterProxyModel() = default;

bool CommandsModelFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, CommandsModel::CommandName).toString();
        const QString rightString = sourceModel()->data(right, CommandsModel::CommandName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}
