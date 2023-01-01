/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonfiltermodel.h"
#include "emoticonmodel.h"

EmoticonFilterModel::EmoticonFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , mEmoticonCategoriesModel(new EmoticonCategoriesModel(this))
{
}

EmoticonFilterModel::~EmoticonFilterModel() = default;

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
