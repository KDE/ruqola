/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TextConverterTest : public QObject
{
    Q_OBJECT
public:
    explicit TextConverterTest(QObject *parent = nullptr);
    ~TextConverterTest() override = default;
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
