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

#include "emoticoncustommodel.h"
#include <KLocalizedString>

EmoticonCustomModel::EmoticonCustomModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EmoticonCustomModel::~EmoticonCustomModel()
{
}

int EmoticonCustomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mCustomRows.count();
}

QVariant EmoticonCustomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mCustomRows.count()) {
        return {};
    }

    if (index.row() < mCustomRows.count()) {
        const auto &row = mCustomRows.at(index.row());
        const CustomEmoji &customEmoti = mCustomEmojiList.at(row.first);
        switch (role) {
        case EmojiName:
            return customEmoti.name(); // Display name for the moment. In the future we need to display "icon"
        case Identifier:
        case Qt::DisplayRole: // for the completion popup (until we have a delegate)
            if (row.second == -1) {
                return customEmoti.emojiIdentifier();
            }
            return customEmoti.aliases().value(row.second);
        }
    }
    return {};
}

const QVector<CustomEmoji> &EmoticonCustomModel::customEmojiList() const
{
    return mCustomEmojiList;
}

void EmoticonCustomModel::setCustomEmojiList(const QVector<CustomEmoji> &newCustomEmojiList)
{
    beginResetModel();
    mCustomEmojiList = newCustomEmojiList;
    mCustomRows.clear();
    int row = 0;
    for (const auto &emoticon : newCustomEmojiList) {
        mCustomRows.append({row, -1});
        const auto numAliases = emoticon.aliases().size();
        for (int i = 0; i < numAliases; ++i) {
            mCustomRows.append({row, i});
        }
        ++row;
    }
    endResetModel();
}
