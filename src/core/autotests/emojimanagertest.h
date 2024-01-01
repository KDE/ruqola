/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class EmojiManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit EmojiManagerTest(QObject *parent = nullptr);
    ~EmojiManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();

    void shouldGenerateHtml();

    void shouldChangeServerUrl();

    void shouldParseEmoji_data();
    void shouldParseEmoji();

    void shouldSupportUnicodeEmojis();
    void shouldOrderUnicodeEmojis();

    void shouldNormalizeReactions_data();
    void shouldNormalizeReactions();

    void shouldDeleteEmojiCustom_data();
    void shouldDeleteEmojiCustom();

    void shouldAddEmojiCustom_data();
    void shouldAddEmojiCustom();

    void shouldUpdateEmojiCustom_data();
    void shouldUpdateEmojiCustom();

    void replaceAsciiEmoji_data();
    void replaceAsciiEmoji();
};
