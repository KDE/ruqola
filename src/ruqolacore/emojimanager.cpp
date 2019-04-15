/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "emojimanager.h"

#include <QJsonObject>
#include <QJsonArray>
#include "ruqola_debug.h"

//TODO cache emoji ?

EmojiManager::EmojiManager(QObject *parent)
    : QObject(parent)
{
    loadUnicodeEmoji();
}

EmojiManager::~EmojiManager()
{
}


void EmojiManager::loadUnicodeEmoji()
{
    //TODO
}

void EmojiManager::loadCustomEmoji(const QJsonObject &obj, bool restApi)
{
    mCustomEmojiList.clear();
    const QJsonArray result = obj.value(restApi ? QLatin1String("emojis") : QLatin1String("result")).toArray();
    for (int i = 0; i < result.size(); i++) {
        const QJsonObject emojiJson = result.at(i).toObject();
        Emoji emoji;
        emoji.parseEmoji(emojiJson, restApi);
        if (emoji.isValid()) {
            mCustomEmojiList.append(emoji);
        }
    }
}

int EmojiManager::count() const
{
    return mCustomEmojiList.count() + mUnicodeEmojiList.count();
}

QString EmojiManager::replaceEmojiIdentifier(const QString &emojiIdentifier)
{
    if (mServerUrl.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "Server Url not defined";
        return emojiIdentifier;
    }
    if (emojiIdentifier.startsWith(QLatin1Char(':')) && emojiIdentifier.endsWith(QLatin1Char(':'))) {
        for (int i = 0, total = mCustomEmojiList.size(); i < total; ++i) {
            if (mCustomEmojiList.at(i).hasEmoji(emojiIdentifier)) {
                QString cachedHtml = mCustomEmojiList.at(i).cachedHtml();
                if (cachedHtml.isEmpty()) {
                    Emoji emoji = mCustomEmojiList[i];
                    cachedHtml = emoji.generateHtmlFromCustomEmoji(mServerUrl);
                    mCustomEmojiList.replace(i, emoji);
                }
                return cachedHtml;
            }
        }
        for (int i = 0, total = mUnicodeEmojiList.size(); i < total; ++i) {
            if (mUnicodeEmojiList.at(i).hasEmoji(emojiIdentifier)) {
                return mUnicodeEmojiList.at(i).unicode();
            }
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Emoji identifier is not correct :" << emojiIdentifier;
    }
    return emojiIdentifier;
}

QString EmojiManager::serverUrl() const
{
    return mServerUrl;
}

void EmojiManager::setServerUrl(const QString &serverUrl)
{
    if (mServerUrl != serverUrl) {
        mServerUrl = serverUrl;
        clearCustomEmojiCachedHtml();
    }
}

void EmojiManager::clearCustomEmojiCachedHtml()
{
    for (int i = 0, total = mCustomEmojiList.size(); i < total; ++i) {
        const QString &cachedHtml = mCustomEmojiList.at(i).cachedHtml();
        if (!cachedHtml.isEmpty()) {
            Emoji emoji = mCustomEmojiList[i];
            emoji.clearCachedHtml();
            mCustomEmojiList.replace(i, emoji);
        }
    }
}
