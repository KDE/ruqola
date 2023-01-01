/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LicensesManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit LicensesManagerTest(QObject *parent = nullptr);
    ~LicensesManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
