/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AuthenticationComboBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit AuthenticationComboBoxTest(QObject *parent = nullptr);
    ~AuthenticationComboBoxTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

