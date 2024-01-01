/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customemoji.h"
#include "libruqolacore_export.h"
#include <QObject>
#include <QRegularExpression>
#include <TextEmoticonsCore/EmoticonCategory>
#include <TextEmoticonsCore/UnicodeEmoticon>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT EmojiManager : public QObject
{
    Q_OBJECT
public:
    explicit EmojiManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~EmojiManager() override;

    void loadCustomEmoji(const QJsonObject &obj);

    [[nodiscard]] int count() const;

    [[nodiscard]] QString replaceEmojiIdentifier(const QString &emojiIdentifier, bool isReaction = false);
    void replaceEmojis(QString *str);
    [[nodiscard]] QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    [[nodiscard]] QList<TextEmoticonsCore::UnicodeEmoticon> unicodeEmojiList() const;
    [[nodiscard]] QList<TextEmoticonsCore::EmoticonCategory> categories() const;
    // Only for test now
    [[nodiscard]] QList<TextEmoticonsCore::UnicodeEmoticon> emojisForCategory(const QString &category) const;

    [[nodiscard]] bool isAnimatedImage(const QString &emojiIdentifier) const;

    [[nodiscard]] TextEmoticonsCore::UnicodeEmoticon unicodeEmoticonForEmoji(const QString &emojiIdentifier) const;

    [[nodiscard]] QString customEmojiFileName(const QString &emojiIdentifier) const;
    [[nodiscard]] QString normalizedReactionEmoji(const QString &emojiIdentifier) const;

    [[nodiscard]] const QVector<CustomEmoji> &customEmojiList() const;

    void addUpdateEmojiCustomList(const QJsonArray &arrayEmojiCustomArray);
    void deleteEmojiCustom(const QJsonArray &obj);
    [[nodiscard]] QString customEmojiFileNameFromIdentifier(const QString &emojiIdentifier) const;

Q_SIGNALS:
    void customEmojiChanged(bool fetchListCustom);

private:
    LIBRUQOLACORE_NO_EXPORT void clearCustomEmojiCachedHtml();
    // Use identifier in a QMap ???
    QVector<CustomEmoji> mCustomEmojiList;
    QString mServerUrl;
    QRegularExpression mReplacePattern;
    RocketChatAccount *const mRocketChatAccount;
    bool mReplacePatternDirty = true;
};
