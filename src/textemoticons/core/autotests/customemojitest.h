/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CustomEmojiTest : public QObject
{
    Q_OBJECT
public:
    explicit CustomEmojiTest(QObject *parent = nullptr);
    ~CustomEmojiTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
