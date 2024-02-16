/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmodel.h"
#include "emoticons/emojimanager.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QIcon>
#include <QUrl>

EmoticonModel::EmoticonModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRocketChatAccount(account)
{
}

EmoticonModel::~EmoticonModel() = default;

int EmoticonModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mUnicodeRows.count() + mCustomRows.count();
}

QIcon EmoticonModel::createCustomIcon(const QString &name) const
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

QVariant EmoticonModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= (mUnicodeRows.count() + mCustomRows.count())) {
        return {};
    }

    if (index.row() < mUnicodeRows.count()) {
        const auto &row = mUnicodeRows.at(index.row());
        if (row.first < mEmoticons.count()) {
            const TextEmoticonsCore::UnicodeEmoticon &unicodeEmoti = mEmoticons.at(row.first);

            switch (role) {
            case CompleterName:
                return unicodeEmoti.identifier().mid(1);
            case Qt::DisplayRole: // for the completion popup (until we have a delegate)
            case UnicodeEmojiRole:
                return unicodeEmoti.unicode();
            case Category:
                return unicodeEmoti.category();
            case IconRole:
                return {};
            case Qt::ToolTipRole:
            case IdentifierRole:
                if (row.second == -1) {
                    return unicodeEmoti.identifier();
                }
                return unicodeEmoti.aliases().value(row.second);
            }
        }
    } else {
        const auto &row = mCustomRows.at(index.row() - mUnicodeRows.count());
        if (row.first < mUnicodeRows.count()) {
            const CustomEmoji &customEmoti = mCustomEmojiList.at(row.first);
            switch (role) {
            case CompleterName:
                return customEmoti.emojiIdentifier().mid(1);
            case Qt::DisplayRole:
            case UnicodeEmojiRole:
                return customEmoti.emojiIdentifier();
            case Category:
                return i18n("Custom");
            case IconRole:
            case Qt::DecorationRole:
                return createCustomIcon(customEmoti.emojiIdentifier());
            case Qt::ToolTipRole:
            case IdentifierRole:
                if (row.second == -1) {
                    return customEmoti.emojiIdentifier();
                }
                return customEmoti.aliases().value(row.second);
            }
        }
    }
    return {};
}

QList<TextEmoticonsCore::UnicodeEmoticon> EmoticonModel::unicodeEmoticons() const
{
    return mEmoticons;
}

void EmoticonModel::setUnicodeEmoticons(const QList<TextEmoticonsCore::UnicodeEmoticon> &emoticons)
{
    beginResetModel();
    mEmoticons = emoticons;
    mUnicodeRows.clear();
    int row = 0;
    for (const auto &emoticon : emoticons) {
        mUnicodeRows.append({row, -1});
        const auto numAliases = emoticon.aliases().size();
        for (int i = 0; i < numAliases; ++i) {
            mUnicodeRows.append({row, i});
        }
        ++row;
    }
    endResetModel();
}

const QList<CustomEmoji> &EmoticonModel::customEmojiList() const
{
    return mCustomEmojiList;
}

void EmoticonModel::setCustomEmojiList(const QList<CustomEmoji> &newCustomEmojiList)
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

#include "moc_emoticonmodel.cpp"
