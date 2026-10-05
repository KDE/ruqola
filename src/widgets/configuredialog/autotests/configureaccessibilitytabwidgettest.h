/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ConfigureAccessibilityTabWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureAccessibilityTabWidgetTest(QObject *parent = nullptr);
    ~ConfigureAccessibilityTabWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
