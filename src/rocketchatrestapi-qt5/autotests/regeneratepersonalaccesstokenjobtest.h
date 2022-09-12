/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RegeneratePersonalAccessTokenJobTest : public QObject
{
    Q_OBJECT
public:
    explicit RegeneratePersonalAccessTokenJobTest(QObject *parent = nullptr);
    ~RegeneratePersonalAccessTokenJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
