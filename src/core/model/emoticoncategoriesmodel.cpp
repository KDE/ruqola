/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticoncategoriesmodel.h"

EmoticonCategoriesModel::EmoticonCategoriesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EmoticonCategoriesModel::~EmoticonCategoriesModel() = default;

int EmoticonCategoriesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mCategories.count();
}

QVariant EmoticonCategoriesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mCategories.count()) {
        return {};
    }
    const EmoticonCategory cat = mCategories.at(index.row());
    switch (role) {
    case Name:
        return cat.name();
    case Category:
        return cat.category();
    }

    return {};
}

void EmoticonCategoriesModel::setCategories(const QVector<EmoticonCategory> &categories)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mCategories.count() - 1);
        mCategories.clear();
        endRemoveRows();
    }
    if (!categories.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, categories.count() - 1);
        mCategories = categories;
        endInsertRows();
    }
}
