/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ForgotPasswordJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ForgotPasswordJobTest(QObject *parent = nullptr);
    ~ForgotPasswordJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
