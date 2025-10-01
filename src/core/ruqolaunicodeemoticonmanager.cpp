/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaunicodeemoticonmanager.h"

using namespace Qt::Literals::StringLiterals;
RuqolaUnicodeEmoticonManager::RuqolaUnicodeEmoticonManager(const QString &filename, QObject *parent)
    : QObject{parent}
    , mUnicodeEmoticonManager(TextEmoticonsCore::UnicodeEmoticonManager::self(filename))
{
}

RuqolaUnicodeEmoticonManager::~RuqolaUnicodeEmoticonManager() = default;

RuqolaUnicodeEmoticonManager *RuqolaUnicodeEmoticonManager::self(const QString &filename)
{
    static RuqolaUnicodeEmoticonManager s_self(filename);
    return &s_self;
}

QList<TextEmoticonsCore::UnicodeEmoticon> RuqolaUnicodeEmoticonManager::unicodeEmojiList() const
{
    return mUnicodeEmoticonManager->unicodeEmojiList();
}

QList<TextEmoticonsCore::EmoticonCategory> RuqolaUnicodeEmoticonManager::categories() const
{
    return mUnicodeEmoticonManager->categories();
}

QList<TextEmoticonsCore::UnicodeEmoticon> RuqolaUnicodeEmoticonManager::emojisForCategory(const QString &category) const
{
    return mUnicodeEmoticonManager->emojisForCategory(category);
}

TextEmoticonsCore::UnicodeEmoticon RuqolaUnicodeEmoticonManager::unicodeEmoticonForEmoji(const QString &emojiIdentifier) const
{
    return TextEmoticonsCore::UnicodeEmoticonManager::self()->unicodeEmoticonForEmoji(emojiIdentifier);
}

int RuqolaUnicodeEmoticonManager::count() const
{
    return TextEmoticonsCore::UnicodeEmoticonManager::self()->count();
}

#include "moc_ruqolaunicodeemoticonmanager.cpp"
