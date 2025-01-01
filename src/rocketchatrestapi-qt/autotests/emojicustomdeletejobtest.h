/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class EmojiCustomDeleteJobTest : public QObject
{
    Q_OBJECT
public:
    explicit EmojiCustomDeleteJobTest(QObject *parent = nullptr);
    ~EmojiCustomDeleteJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
