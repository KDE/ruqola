/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojimodel.h"
#include "textemoticonscore_debug.h"
#include <TextEmoticonsCore/CustomEmojiIconManager>
using namespace TextEmoticonsCore;
EmojiModel::EmojiModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EmojiModel::~EmojiModel() = default;

int EmojiModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mEmoticonList.count() + mCustomEmojiList.count();
}

QVariant EmojiModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= (mEmoticonList.count() + mCustomEmojiList.count())) {
        return {};
    }
    if (index.row() < mEmoticonList.count()) {
        const auto &unicodeEmoti = mEmoticonList.at(index.row());
        switch (role) {
        case Qt::DisplayRole:
        case UnicodeEmoji:
            return unicodeEmoti.unicode();
        case Category:
            return unicodeEmoti.category();
        case Order:
            return unicodeEmoti.order();
        case Identifier:
        case Qt::ToolTipRole:
            return unicodeEmoti.identifier();
        }
    } else {
        const auto &customEmoji = mCustomEmojiList.at(index.row() - mEmoticonList.count());
        switch (role) {
        case Qt::DecorationRole: {
            if (mCustomEmojiIconManager) {
                if (customEmoji.isAnimatedEmoji()) {
                    const QString filename = mCustomEmojiIconManager->fileName(customEmoji.identifier());
                    if (!filename.isEmpty()) {
                        const QIcon icon(filename);
                        return icon;
                    }
                }
                const QIcon icon = mCustomEmojiIconManager->generateIcon(customEmoji.identifier());
                return icon;
            } else {
                qCWarning(TEXTEMOTICONSCORE_LOG) << "mCustomEmojiIconManager is null. It's a bug";
            }
            return {};
        }
        case Category:
            return customEmoji.category();
        case Order:
            return -1;
        case Qt::ToolTipRole:
        case Identifier:
        case UnicodeEmoji:
            return customEmoji.identifier();
        }
    }
    return {};
}

const QList<TextEmoticonsCore::UnicodeEmoticon> &EmojiModel::emoticonList() const
{
    return mEmoticonList;
}

void EmojiModel::setUnicodeEmoticonList(const QList<TextEmoticonsCore::UnicodeEmoticon> &newEmoticonList)
{
    beginResetModel();
    mEmoticonList = newEmoticonList;
    endResetModel();
}

QList<TextEmoticonsCore::CustomEmoji> EmojiModel::customEmojiList() const
{
    return mCustomEmojiList;
}

void EmojiModel::setCustomEmojiList(const QList<TextEmoticonsCore::CustomEmoji> &newCustomEmojiList)
{
    beginResetModel();
    mCustomEmojiList = newCustomEmojiList;
    endResetModel();
}

TextEmoticonsCore::CustomEmojiIconManager *EmojiModel::customEmojiIconManager() const
{
    return mCustomEmojiIconManager;
}

void EmojiModel::setCustomEmojiIconManager(TextEmoticonsCore::CustomEmojiIconManager *newCustomEmojiIconManager)
{
    mCustomEmojiIconManager = newCustomEmojiIconManager;
}
