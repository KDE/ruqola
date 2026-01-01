/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class WelcomeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit WelcomeWidgetTest(QObject *parent = nullptr);
    ~WelcomeWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSignal();
};
