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

#pragma once

#include "customemoji.h"
#include "emoticoncategory.h"
#include "libruqolacore_export.h"
#include "unicodeemoticon.h"
#include <QObject>
#include <QRegularExpression>

class LIBRUQOLACORE_EXPORT EmojiManager : public QObject
{
    Q_OBJECT
public:
    explicit EmojiManager(QObject *parent = nullptr);
    ~EmojiManager() override;

    void loadCustomEmoji(const QJsonObject &obj);

    Q_REQUIRED_RESULT int count() const;

    Q_REQUIRED_RESULT QString replaceEmojiIdentifier(const QString &emojiIdentifier, bool isReaction = false);
    void replaceEmojis(QString *str);
    Q_REQUIRED_RESULT QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    Q_REQUIRED_RESULT QVector<UnicodeEmoticon> unicodeEmojiList() const;
    Q_REQUIRED_RESULT QVector<EmoticonCategory> categories() const;
    Q_REQUIRED_RESULT QVector<UnicodeEmoticon> emojisForCategory(const QString &category) const;

    Q_REQUIRED_RESULT bool isAnimatedImage(const QString &emojiIdentifier) const;

    Q_REQUIRED_RESULT UnicodeEmoticon unicodeEmoticonForEmoji(const QString &emojiIdentifier) const;

    Q_REQUIRED_RESULT QString customEmojiFileName(const QString &emojiIdentifier) const;
    Q_REQUIRED_RESULT QString normalizedReactionEmoji(const QString &emojiIdentifier) const;

    Q_REQUIRED_RESULT const QVector<CustomEmoji> &customEmojiList() const;

private:
    Q_DISABLE_COPY(EmojiManager)
    void clearCustomEmojiCachedHtml();
    // Use identifier in a QMap ???
    QVector<CustomEmoji> mCustomEmojiList;
    QString mServerUrl;
    QRegularExpression mReplacePattern;
    bool mReplacePatternDirty = true;
};

