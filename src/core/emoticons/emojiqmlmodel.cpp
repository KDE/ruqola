/*
   Copyright (c) 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>

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

#include "emojiqmlmodel.h"

#include "emojimanager.h"

EmojiQmlModel::EmojiQmlModel(EmojiManager *emojiManager, QObject *parent)
    : QAbstractListModel(parent)
    , mEmojiManager(emojiManager)
{
    mCategories = mEmojiManager->categories();
    for (int i = 0; i < mCategories.count(); i++) {
        mCategoryModels.push_back(new EmojiCategoryQmlModel(this, mCategories[i].category(), this));
    }
    qDebug() << mCategories;
}

EmojiQmlModel::~EmojiQmlModel() = default;

int EmojiQmlModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    qDebug() << "ASDFASDFASD" << mCategories.size();

    return mCategories.size();
}

QVariant EmojiQmlModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    switch (static_cast<Roles>(role))
    {
        case Roles::CategoryName:
            return mCategories.at(row).category();
        case Roles::CategoryEmoticon:
            return mCategories.at(row).name();
        case Roles::CategoryModel:
            return QVariant::fromValue(mCategoryModels.at(row));
    }

    return {};
}

QHash<int, QByteArray> EmojiQmlModel::roleNames() const
{
    return {
        {static_cast<int>(Roles::CategoryName), QByteArrayLiteral("categoryName")},
        {static_cast<int>(Roles::CategoryEmoticon), QByteArrayLiteral("categoryEmoticon")},
        {static_cast<int>(Roles::CategoryModel), QByteArrayLiteral("categoryModel")}
    };
}

EmojiCategoryQmlModel::EmojiCategoryQmlModel(EmojiQmlModel *emojiModel, const QString &category, QObject *parent)
    : QAbstractListModel(parent)
    , mEmojiQmlModel(emojiModel)
    , mCategory(category)
{
    mEmoticons = mEmojiQmlModel->mEmojiManager->emojisForCategory(category);
    qDebug() << mEmoticons[0].aliases();
}

EmojiCategoryQmlModel::~EmojiCategoryQmlModel() = default;

int EmojiCategoryQmlModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return mEmoticons.count();
}

QVariant EmojiCategoryQmlModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    switch (static_cast<Roles>(role)) {
        case Roles::EmojiUnicode:
            return mEmoticons[row].unicode();
        case Roles::EmojiUnicodeDisplay:
            return mEmoticons[row].unicodeDisplay();
        case Roles::EmojiAliases:
            return mEmoticons[row].aliases();
        case Roles::EmojiCategory:
            return mEmoticons[row].category();
        case Roles::EmojiIdentifier:
            return mEmoticons[row].identifier();
    }

    return {};
}

QHash<int, QByteArray> EmojiCategoryQmlModel::roleNames() const
{
    return {
        {static_cast<int>(Roles::EmojiUnicode), QByteArrayLiteral("unicode")},
        {static_cast<int>(Roles::EmojiUnicodeDisplay), QByteArrayLiteral("unicodeDisplay")},
        {static_cast<int>(Roles::EmojiAliases), QByteArrayLiteral("aliases")},
        {static_cast<int>(Roles::EmojiCategory), QByteArrayLiteral("category")},
        {static_cast<int>(Roles::EmojiIdentifier), QByteArrayLiteral("identifier")}
    };
}