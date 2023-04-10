/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticoncustommodel.h"
#include "emoticons/emojimanager.h"
#include "rocketchataccount.h"
#include <QIcon>
#include <QUrl>

EmoticonCustomModel::EmoticonCustomModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRocketChatAccount(account)
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
        const CustomEmoji &customEmoji = mCustomEmojiList.at(row.first);
        switch (role) {
        case EmojiName:
            return customEmoji.name(); // Display name for the moment. In the future we need to display "icon"
        case Qt::DecorationRole:
            return generateIcon(customEmoji.emojiIdentifier());
        case Identifier:
        case Qt::DisplayRole: // for the completion popup (until we have a delegate)
            if (row.second == -1) {
                return customEmoji.emojiIdentifier();
            }
            return customEmoji.aliases().value(row.second);
        }
    }
    return {};
}

QIcon EmoticonCustomModel::generateIcon(const QString &name) const
{
    if (mRocketChatAccount) {
        const QString fileName = mRocketChatAccount->emojiManager()->customEmojiFileName(name);
        if (!fileName.isEmpty()) {
            const QUrl emojiUrl = mRocketChatAccount->attachmentUrlFromLocalCache(fileName);
            if (!emojiUrl.isEmpty()) {
                const QIcon icon(emojiUrl.toLocalFile());
                return icon;
            }
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
