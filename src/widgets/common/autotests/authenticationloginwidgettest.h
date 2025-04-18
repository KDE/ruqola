/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AuthenticationLoginWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AuthenticationLoginWidgetTest(QObject *parent = nullptr);
    ~AuthenticationLoginWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSignalWhenPressEnter();
};
