/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticoncustommodel.h"

EmoticonCustomModel::EmoticonCustomModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EmoticonCustomModel::~EmoticonCustomModel() = default;

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
