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

#pragma once

#include <QAbstractListModel>

#include "emojimanager.h"

class EmojiCategoryQmlModel;

// Emoji model contains categories, categories contain emoji entries

class LIBRUQOLACORE_EXPORT EmojiQmlModel : public QAbstractListModel
{
    Q_OBJECT

    enum class Roles {
        CategoryName,
        CategoryEmoticon,
        CategoryModel,
    };

public:
    explicit EmojiQmlModel(EmojiManager *emojiManager, QObject *parent = nullptr);
    ~EmojiQmlModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const;

private:
    friend class EmojiCategoryQmlModel;
    EmojiManager *mEmojiManager = nullptr;
    QVector<EmoticonCategory> mCategories;
    QVector<EmojiCategoryQmlModel*> mCategoryModels;
};

class LIBRUQOLACORE_EXPORT EmojiCategoryQmlModel : public QAbstractListModel
{
    Q_OBJECT

    enum class Roles {
        EmojiUnicode,
        EmojiAliases,
        EmojiUnicodeDisplay,
        EmojiCategory,
        EmojiIdentifier,
    };

public:
    explicit EmojiCategoryQmlModel(EmojiQmlModel *emojiModel, const QString &category, QObject *parent = nullptr);
    ~EmojiCategoryQmlModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const;

private:
    EmojiQmlModel *mEmojiQmlModel = nullptr;
    const QString mCategory;
    QVector<UnicodeEmoticon> mEmoticons;
};

Q_DECLARE_METATYPE(EmojiQmlModel*)
Q_DECLARE_METATYPE(EmojiCategoryQmlModel*)
