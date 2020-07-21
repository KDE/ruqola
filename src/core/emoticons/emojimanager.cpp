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

#include "emoticons/emojimanager.h"
#include "emoticons/unicodeemoticonmanager.h"
#include "ruqola_debug.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

EmojiManager::EmojiManager(QObject *parent)
    : QObject(parent)
{
}

EmojiManager::~EmojiManager()
{
}

QVector<UnicodeEmoticon> EmojiManager::unicodeEmojiList() const
{
    return UnicodeEmoticonManager::self()->unicodeEmojiList();
}

QVector<EmoticonCategory> EmojiManager::categories() const
{
    return UnicodeEmoticonManager::self()->categories();
}

QVector<UnicodeEmoticon> EmojiManager::emojisForCategory(const QString &category) const
{
    return UnicodeEmoticonManager::self()->emojisForCategory(category);
}

void EmojiManager::loadCustomEmoji(const QJsonObject &obj)
{
    mCustomEmojiList.clear();
    const QJsonObject result = obj.value(QLatin1String("emojis")).toObject();
    const QJsonArray array = result.value(QLatin1String("update")).toArray();
    //TODO add support for remove when we store it in local
    for (int i = 0, total = array.size(); i < total; ++i) {
        const QJsonObject emojiJson = array.at(i).toObject();
        CustomEmoji emoji;
        emoji.parseEmoji(emojiJson);
        if (emoji.isValid()) {
            mCustomEmojiList.append(emoji);
        }
    }
}

int EmojiManager::count() const
{
    return mCustomEmojiList.count() + UnicodeEmoticonManager::self()->count();
}

bool EmojiManager::isAnimatedImage(const QString &emojiIdentifier) const
{
    if (emojiIdentifier.startsWith(QLatin1Char(':')) && emojiIdentifier.endsWith(QLatin1Char(':'))) {
        for (int i = 0, total = mCustomEmojiList.size(); i < total; ++i) {
            const CustomEmoji emoji = mCustomEmojiList.at(i);
            if (emoji.hasEmoji(emojiIdentifier)) {
                return emoji.isAnimatedImage();
            }
        }
    }
    return false;
}

UnicodeEmoticon EmojiManager::unicodeEmoticonForEmoji(const QString &emojiIdentifier) const
{
    return UnicodeEmoticonManager::self()->unicodeEmoticonForEmoji(emojiIdentifier);
}

QString EmojiManager::customEmojiFileName(const QString &emojiIdentifier) const
{
    for (const CustomEmoji &customEmoji : mCustomEmojiList) {
        if (customEmoji.hasEmoji(emojiIdentifier)) {
            return customEmoji.emojiFileName();
        }
    }
    return {};
}

QString EmojiManager::replaceEmojiIdentifier(const QString &emojiIdentifier, bool isReaction)
{
    if (mServerUrl.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "Server Url not defined";
        return emojiIdentifier;
    }
    if (emojiIdentifier.startsWith(QLatin1Char(':')) && emojiIdentifier.endsWith(QLatin1Char(':'))) {
        for (CustomEmoji &emoji : mCustomEmojiList) {
            if (emoji.hasEmoji(emojiIdentifier)) {
                QString cachedHtml = emoji.cachedHtml();
                if (cachedHtml.isEmpty()) {
                    //For the moment we can't support animated image as emoticon in text. Only as Reaction.
                    if (emoji.isAnimatedImage() && isReaction) {
                        cachedHtml = emoji.generateAnimatedUrlFromCustomEmoji(mServerUrl);
                    } else {
                        cachedHtml = emoji.generateHtmlFromCustomEmoji(mServerUrl);
                    }
                }
                return cachedHtml;
            }
        }
        const UnicodeEmoticon unicodeEmoticon = unicodeEmoticonForEmoji(emojiIdentifier);
        if (unicodeEmoticon.isValid()) {
            return unicodeEmoticon.unicodeDisplay();
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Emoji identifier is not correct :" << emojiIdentifier;
    }
    return emojiIdentifier;
}

void EmojiManager::replaceEmojis(QString *str)
{
    static const QRegularExpression pattern(QStringLiteral(":\\w+:"));
    int offset = 0;
    while (offset < str->size()) {
        const auto match = pattern.match(str, offset);
        if (!match.hasMatch()) {
            break;
        }
        const auto word = match.captured();
        const auto replaceWord = replaceEmojiIdentifier(word);
        str->replace(match.capturedStart(), word.size(), replaceWord);
        offset = match.capturedStart() + replaceWord.size();
    }
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
        mCustomEmojiList[i].clearCachedHtml();
    }
}
