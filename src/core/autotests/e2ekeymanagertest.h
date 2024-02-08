/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class E2eKeyManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eKeyManagerTest(QObject *parent = nullptr);
    ~E2eKeyManagerTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
