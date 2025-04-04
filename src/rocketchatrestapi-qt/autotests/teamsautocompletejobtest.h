/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamsAutoCompleteJobTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamsAutoCompleteJobTest(QObject *parent = nullptr);
    ~TeamsAutoCompleteJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
