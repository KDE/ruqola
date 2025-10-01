/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <TextEmoticonsCore/UnicodeEmoticonManager>
class RuqolaUnicodeEmoticonManager : public QObject
{
    Q_OBJECT
public:
    explicit RuqolaUnicodeEmoticonManager(QObject *parent = nullptr);
    ~RuqolaUnicodeEmoticonManager() override;

    static RuqolaUnicodeEmoticonManager *self();

    [[nodiscard]] QList<TextEmoticonsCore::UnicodeEmoticon> unicodeEmojiList() const;
    [[nodiscard]] QList<TextEmoticonsCore::EmoticonCategory> categories() const;
    [[nodiscard]] QList<TextEmoticonsCore::UnicodeEmoticon> emojisForCategory(const QString &category) const;
    [[nodiscard]] TextEmoticonsCore::UnicodeEmoticon unicodeEmoticonForEmoji(const QString &emojiIdentifier) const;
    [[nodiscard]] int count() const;

    void loadUnicodeEmoji(const QString &filename);

private:
    TextEmoticonsCore::UnicodeEmoticonManager *const mUnicodeEmoticonManager;
};
