/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticoncustommodelfilterproxymodel.h"
#include "emoticoncustommodel.h"

EmoticonCustomModelFilterProxyModel::EmoticonCustomModelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(EmoticonCustomModel::Identifier);
    sort(0);
}

EmoticonCustomModelFilterProxyModel::~EmoticonCustomModelFilterProxyModel() = default;

bool EmoticonCustomModelFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, EmoticonCustomModel::Identifier).toString();
        const QString rightString = sourceModel()->data(right, EmoticonCustomModel::Identifier).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}
