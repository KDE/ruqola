/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LicensesListJobTest : public QObject
{
    Q_OBJECT
public:
    explicit LicensesListJobTest(QObject *parent = nullptr);
    ~LicensesListJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
