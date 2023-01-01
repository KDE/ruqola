/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmodelfilterproxymodel.h"
#include "emoticonmodel.h"

EmoticonModelFilterProxyModel::EmoticonModelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(EmoticonModel::Identifier);
    sort(0);
}

EmoticonModelFilterProxyModel::~EmoticonModelFilterProxyModel() = default;

bool EmoticonModelFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, EmoticonModel::Identifier).toString();
        const QString rightString = sourceModel()->data(right, EmoticonModel::Identifier).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}
