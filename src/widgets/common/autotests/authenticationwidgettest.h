/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class AuthenticationWidgetTest : public QWidget
{
    Q_OBJECT
public:
    explicit AuthenticationWidgetTest(QWidget *parent = nullptr);
    ~AuthenticationWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
