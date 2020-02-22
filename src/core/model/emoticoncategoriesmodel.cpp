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

#include "emoticoncategoriesmodel.h"

EmoticonCategoriesModel::EmoticonCategoriesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EmoticonCategoriesModel::~EmoticonCategoriesModel()
{
}

int EmoticonCategoriesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0; // flat model
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

QHash<int, QByteArray> EmoticonCategoriesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = QByteArrayLiteral("name");
    roles[Category] = QByteArrayLiteral("category");
    //Add tooltip ? we need to extract info from json file.
    return roles;
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
