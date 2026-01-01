/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ApplicationsSettingsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsWidgetTest(QObject *parent = nullptr);
    ~ApplicationsSettingsWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
