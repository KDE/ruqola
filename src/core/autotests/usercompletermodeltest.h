/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UserCompleterModelTest : public QObject
{
    Q_OBJECT
public:
    explicit UserCompleterModelTest(QObject *parent = nullptr);
    ~UserCompleterModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddValues();
    void shouldVerifyData();
};

