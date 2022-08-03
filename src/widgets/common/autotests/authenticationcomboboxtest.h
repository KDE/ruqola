/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
