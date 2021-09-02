/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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
#include <KLocalizedString>

EmoticonModel::EmoticonModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EmoticonModel::~EmoticonModel()
{
}

int EmoticonModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mUnicodeRows.count() + mCustomRows.count();
}

QVariant EmoticonModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= (mUnicodeRows.count() + mCustomRows.count())) {
        return {};
    }

    if (index.row() < mUnicodeRows.count()) {
        const auto &row = mUnicodeRows.at(index.row());
        if (row.first < mEmoticons.count()) {
            const UnicodeEmoticon &unicodeEmoti = mEmoticons.at(row.first);

            switch (role) {
            case CompleterName:
                return unicodeEmoti.identifier().mid(1);
            case UnicodeEmoji:
                return unicodeEmoti.unicode();
            case Category:
                return unicodeEmoti.category();
            case Identifier:
            case Qt::DisplayRole: // for the completion popup (until we have a delegate)
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
            case UnicodeEmoji:
                return customEmoti.name(); // Display name for the moment. In the future we need to display "icon"
            case Category:
                return i18n("Custom");
            case Identifier:
            case Qt::DisplayRole: // for the completion popup (until we have a delegate)
                if (row.second == -1) {
                    return customEmoti.emojiIdentifier();
                }
                return customEmoti.aliases().value(row.second);
            }
        }
    }
    return {};
}

QVector<UnicodeEmoticon> EmoticonModel::unicodeEmoticons() const
{
    return mEmoticons;
}

void EmoticonModel::setUnicodeEmoticons(const QVector<UnicodeEmoticon> &emoticons)
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

const QVector<CustomEmoji> &EmoticonModel::customEmojiList() const
{
    return mCustomEmojiList;
}

void EmoticonModel::setCustomEmojiList(const QVector<CustomEmoji> &newCustomEmojiList)
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
