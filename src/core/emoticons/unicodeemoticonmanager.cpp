/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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


#include "unicodeemoticonmanager.h"
#include "unicodeemoticonparser.h"
#include "ruqola_debug.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

UnicodeEmoticonManager::UnicodeEmoticonManager(QObject *parent)
    : QObject(parent)
{
    loadUnicodeEmoji();
}

UnicodeEmoticonManager::~UnicodeEmoticonManager()
{

}

UnicodeEmoticonManager *UnicodeEmoticonManager::self()
{
    static UnicodeEmoticonManager s_self;
    return &s_self;
}

void UnicodeEmoticonManager::loadUnicodeEmoji()
{
    UnicodeEmoticonParser unicodeParser;
    QFile file(QStringLiteral(":/emoji.json"));
    if (!file.open(QFile::ReadOnly)) {
        qCWarning(RUQOLA_LOG) << "Impossible to open file: " << file.errorString();
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    const QJsonObject obj = doc.object();
    mUnicodeEmojiList = unicodeParser.parse(obj);
}

QVector<UnicodeEmoticon> UnicodeEmoticonManager::unicodeEmojiList() const
{
    return mUnicodeEmojiList;
}

int UnicodeEmoticonManager::count() const
{
    return mUnicodeEmojiList.count();
}

QVector<EmoticonCategory> UnicodeEmoticonManager::categories() const
{
    QVector<EmoticonCategory> categories;
    QSet<QString> seen;
    for (const UnicodeEmoticon &emo : qAsConst(mUnicodeEmojiList)) {
        // Pick the first icon in each category
        const QString category = emo.category();
        if (!seen.contains(category)) {
            seen.insert(category);
            if (category == QLatin1String("modifier")) { // pointless icons
                continue;
            }
            EmoticonCategory cat;
            cat.setCategory(category);
            cat.setName(emo.unicode());
            categories.append(std::move(cat));
        }
    }
    // TODO sort categories in a way that makes sense for the user
    return categories;
}

QVector<UnicodeEmoticon> UnicodeEmoticonManager::emojisForCategory(const QString &category) const
{
    QVector<UnicodeEmoticon> result;

    auto hasRequestedCategory = [category](const UnicodeEmoticon &emo) {
                                    return emo.category() == category;
                                };
    std::copy_if(mUnicodeEmojiList.begin(), mUnicodeEmojiList.end(),
                 std::back_inserter(result), hasRequestedCategory);
    return result;
}

UnicodeEmoticon UnicodeEmoticonManager::unicodeEmoticonForEmoji(const QString &emojiIdentifier) const
{
    for (const UnicodeEmoticon &emo : mUnicodeEmojiList) {
        if (emo.hasEmoji(emojiIdentifier)) {
            return emo;
        }
    }
    return {};
}

