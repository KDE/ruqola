/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AppInstalledJobTest : public QObject
{
    Q_OBJECT
public:
    explicit AppInstalledJobTest(QObject *parent = nullptr);
    ~AppInstalledJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
