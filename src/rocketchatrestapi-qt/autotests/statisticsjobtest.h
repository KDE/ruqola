/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class StatisticsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit StatisticsJobTest(QObject *parent = nullptr);
    ~StatisticsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
