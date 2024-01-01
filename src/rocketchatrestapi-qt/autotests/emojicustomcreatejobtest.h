/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class EmojiCustomCreateJobTest : public QObject
{
    Q_OBJECT
public:
    explicit EmojiCustomCreateJobTest(QObject *parent = nullptr);
    ~EmojiCustomCreateJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
