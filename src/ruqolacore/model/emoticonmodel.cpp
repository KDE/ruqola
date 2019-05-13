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

#include "emoticonmodel.h"

EmoticonModel::EmoticonModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EmoticonModel::~EmoticonModel()
{
}

int EmoticonModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (mEmoticons.contains(mCurrentCategory)) {
        return mEmoticons[mCurrentCategory].count();
    }
    return 0;
}

QVariant EmoticonModel::data(const QModelIndex &index, int role) const
{
    const QVector<UnicodeEmoticon> emoticonsFromCategoryList = mEmoticons.value(mCurrentCategory);
    if (index.row() < 0 || index.row() >= emoticonsFromCategoryList.count()) {
        return {};
    }
    const UnicodeEmoticon unicodeEmoti = emoticonsFromCategoryList.at(index.row());
    switch (role) {
    case Identifier:
        return unicodeEmoti.identifier();
    case Text:
        return unicodeEmoti.key();
    case UnicodeEmoji:
        return unicodeEmoti.unicode();
    case Order:
        return unicodeEmoti.order();
    }
    return {};
}

QHash<int, QByteArray> EmoticonModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Identifier] = QByteArrayLiteral("identifier");
    roles[Text] = QByteArrayLiteral("text");
    roles[UnicodeEmoji] = QByteArrayLiteral("unicodeEmoji");
    roles[Order] = QByteArrayLiteral("order");
    return roles;
}

QMap<QString, QVector<UnicodeEmoticon> > EmoticonModel::emoticons() const
{
    return mEmoticons;
}

void EmoticonModel::setEmoticons(const QMap<QString, QVector<UnicodeEmoticon> > &emoticons)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mEmoticons.count() - 1);
        mEmoticons.clear();
        endRemoveRows();
    }
    if (!mEmoticons.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mEmoticons.count() - 1);
        mEmoticons = emoticons;
        endInsertRows();
    }
    if (!mEmoticons.isEmpty()) {
        mCurrentCategory = mEmoticons.keys().at(0);
    }
}

void EmoticonModel::setCurrentCategory(const QString &category)
{
    if (mCurrentCategory != category) {
        beginResetModel();
        mCurrentCategory = category;
        endResetModel();
    }
}

QString EmoticonModel::currentCategory() const
{
    return mCurrentCategory;
}

//TODO return list of categories
