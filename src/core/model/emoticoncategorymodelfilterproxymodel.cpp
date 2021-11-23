/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "emoticoncategorymodelfilterproxymodel.h"
#include "emoticonmodel.h"

EmoticonCategoryModelFilterProxyModel::EmoticonCategoryModelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(EmoticonModel::Identifier);
    sort(0);
}

EmoticonCategoryModelFilterProxyModel::~EmoticonCategoryModelFilterProxyModel() = default;

bool EmoticonCategoryModelFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
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

const QString &EmoticonCategoryModelFilterProxyModel::category() const
{
    return mCategory;
}

void EmoticonCategoryModelFilterProxyModel::setCategory(const QString &newCategory)
{
    mCategory = newCategory;
}

bool EmoticonCategoryModelFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mCategory.isEmpty()) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString category = sourceIndex.data(EmoticonModel::Category).toString();
    if (mCategory == category) {
        return true;
    }
    return false;
}
