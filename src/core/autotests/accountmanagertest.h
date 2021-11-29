/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AccountManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit AccountManagerTest(QObject *parent = nullptr);
    ~AccountManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddAccount();
};

