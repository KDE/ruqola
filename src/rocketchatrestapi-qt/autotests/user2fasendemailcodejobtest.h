/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class User2FASendEmailCodeJobTest : public QObject
{
    Q_OBJECT
public:
    explicit User2FASendEmailCodeJobTest(QObject *parent = nullptr);
    ~User2FASendEmailCodeJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
