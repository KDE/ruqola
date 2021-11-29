/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class EmojiCustomUpdateJobTest : public QObject
{
    Q_OBJECT
public:
    explicit EmojiCustomUpdateJobTest(QObject *parent = nullptr);
    ~EmojiCustomUpdateJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};

