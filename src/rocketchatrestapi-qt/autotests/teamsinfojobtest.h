/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamsInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamsInfoJobTest(QObject *parent = nullptr);
    ~TeamsInfoJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
