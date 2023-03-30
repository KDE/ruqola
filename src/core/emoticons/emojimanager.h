/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customemoji.h"
#include "emoticoncategory.h"
#include "libruqolacore_export.h"
#include "unicodeemoticon.h"
#include <QObject>
#include <QRegularExpression>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT EmojiManager : public QObject
{
    Q_OBJECT
public:
    explicit EmojiManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~EmojiManager() override;

    void loadCustomEmoji(const QJsonObject &obj);

    Q_REQUIRED_RESULT int count() const;

    Q_REQUIRED_RESULT QString replaceEmojiIdentifier(const QString &emojiIdentifier, bool isReaction = false);
    void replaceEmojis(QString *str);
    Q_REQUIRED_RESULT QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    Q_REQUIRED_RESULT QVector<UnicodeEmoticon> unicodeEmojiList() const;
    Q_REQUIRED_RESULT QVector<EmoticonCategory> categories() const;
    // Only for test now
    Q_REQUIRED_RESULT QVector<UnicodeEmoticon> emojisForCategory(const QString &category) const;

    Q_REQUIRED_RESULT bool isAnimatedImage(const QString &emojiIdentifier) const;

    Q_REQUIRED_RESULT UnicodeEmoticon unicodeEmoticonForEmoji(const QString &emojiIdentifier) const;

    Q_REQUIRED_RESULT QString customEmojiFileName(const QString &emojiIdentifier) const;
    Q_REQUIRED_RESULT QString normalizedReactionEmoji(const QString &emojiIdentifier) const;

    Q_REQUIRED_RESULT const QVector<CustomEmoji> &customEmojiList() const;

    void addUpdateEmojiCustomList(const QJsonArray &arrayEmojiCustomArray);
    void deleteEmojiCustom(const QJsonArray &obj);
    Q_REQUIRED_RESULT QString customEmojiFileNameFromIdentifier(const QString &emojiIdentifier) const;

Q_SIGNALS:
    void customEmojiChanged(bool fetchListCustom);

private:
    Q_DISABLE_COPY(EmojiManager)
    void clearCustomEmojiCachedHtml();
    // Use identifier in a QMap ???
    QVector<CustomEmoji> mCustomEmojiList;
    QString mServerUrl;
    QRegularExpression mReplacePattern;
    RocketChatAccount *const mRocketChatAccount;
    bool mReplacePatternDirty = true;
};
