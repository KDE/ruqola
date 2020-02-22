/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "emoticonfiltermodel.h"
#include "emoticonmodel.h"

EmoticonFilterModel::EmoticonFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    mEmoticonCategoriesModel = new EmoticonCategoriesModel(this);
}

EmoticonFilterModel::~EmoticonFilterModel()
{
}

void EmoticonFilterModel::setCurrentCategory(const QString &category)
{
    if (mCurrentCategory != category) {
        mCurrentCategory = category;
        invalidateFilter();
    }
}

QString EmoticonFilterModel::currentCategory() const
{
    return mCurrentCategory;
}

EmoticonCategoriesModel *EmoticonFilterModel::emoticonCategoriesModel() const
{
    return mEmoticonCategoriesModel;
}

bool EmoticonFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!mCurrentCategory.isEmpty()) {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        const QString category = sourceIndex.data(EmoticonModel::Category).toString();
        return mCurrentCategory == category;
    }
    return true;
}
