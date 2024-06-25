/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TextConverterCMarkTest : public QObject
{
    Q_OBJECT
public:
    explicit TextConverterCMarkTest(QObject *parent = nullptr);
    ~TextConverterCMarkTest() override = default;
private Q_SLOTS:
    void shouldConvertText_data();
    void shouldConvertText();

    void shouldHighlightWords_data();
    void shouldHighlightWords();

    void shouldHighlightText_data();
    void shouldHighlightText();

    void shouldConvertTextWithEmoji_data();
    void shouldConvertTextWithEmoji();

    void shouldShowChannels_data();
    void shouldShowChannels();

    void shouldShowUsers_data();
    void shouldShowUsers();

    void shouldShowSearchedText_data();
    void shouldShowSearchedText();
};
