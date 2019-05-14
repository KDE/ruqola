/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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
    Q_UNUSED(parent);
    //return mCategories.count();
    return 0;
}

QVariant EmoticonCategoriesModel::data(const QModelIndex &index, int role) const
{
//    if (index.row() < 0 || index.row() >= mCategories.count()) {
//        return {};
//    }
//    const Category cat = mCategories.at(index.row());

    //TODO
    return {};

//    const QVector<UnicodeEmoticon> emoticonsFromCategoryList = mEmoticons.value(mCurrentCategory);
//    if (index.row() < 0 || index.row() >= emoticonsFromCategoryList.count()) {
//        return {};
//    }
}

QHash<int, QByteArray> EmoticonCategoriesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = QByteArrayLiteral("name");
    roles[Category] = QByteArrayLiteral("category");
    return roles;
}

//void EmoticonCategoriesModel::setEmoticons(const QMap<QString, QVector<UnicodeEmoticon> > &emoticons)
//{
////    if (rowCount() != 0) {
////        beginRemoveRows(QModelIndex(), 0, mEmoticons.count() - 1);
////        mEmoticons.clear();
////        endRemoveRows();
////    }
////    if (!emoticons.isEmpty()) {
////        beginInsertRows(QModelIndex(), 0, mEmoticons.count() - 1);
////        mEmoticons = emoticons;
////        endInsertRows();
////    }
////    if (!mEmoticons.isEmpty()) {
////        mCurrentCategory = mEmoticons.keys().at(0);
////        mCategories = mEmoticons.keys();
////    }
//}

