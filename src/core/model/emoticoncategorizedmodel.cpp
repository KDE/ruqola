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

#include "emoticoncategorizedmodel.h"

#include <KCategorizedSortFilterProxyModel>

EmoticonCategorizedModel::EmoticonCategorizedModel(QObject *parent)
    : QAbstractListModel(parent),
      mEmojiFont(QStringLiteral("NotoColorEmoji"), 22)
{
}

EmoticonCategorizedModel::~EmoticonCategorizedModel()
{
}

int EmoticonCategorizedModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0; // flat model
    return mEmoticons.count();
}

QVariant EmoticonCategorizedModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mEmoticons.count()) {
        return {};
    }
    const UnicodeEmoticon &unicodeEmoti = mEmoticons.at(index.row());
    switch (role) {
    case Identifier:
        return unicodeEmoti.identifier();
    case Text:
        return unicodeEmoti.key();
    case UnicodeEmoji:
    case Qt::DisplayRole:
        return unicodeEmoti.unicode();
    case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
    case KCategorizedSortFilterProxyModel::CategorySortRole:
        return unicodeEmoti.category();
    case Qt::FontRole:
        return mEmojiFont;
    }

    return {};
}

void EmoticonCategorizedModel::setEmoticons(const QMap<QString, QVector<UnicodeEmoticon> > &emoticons)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mEmoticons.count() - 1);
        mEmoticons.clear();
        endRemoveRows();
    }

    // Flatten the map
    QVector<UnicodeEmoticon> fullVector;
    auto addVectorSize = [](int current, const QVector<UnicodeEmoticon> &vec) { return current + vec.size(); };
    const int size = std::accumulate(emoticons.begin(), emoticons.end(), 0, addVectorSize);
    qDebug() << "accumulated" << size;
    fullVector.reserve(size);
    for (auto it = emoticons.begin(); it != emoticons.end(); ++it) {
        fullVector += it.value();
    }
    Q_ASSERT(fullVector.size() == size);

    if (!emoticons.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, fullVector.count() - 1);
        mEmoticons = fullVector;
        endInsertRows();
    }
}
