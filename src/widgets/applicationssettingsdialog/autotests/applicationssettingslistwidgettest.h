/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ApplicationsSettingsListWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListWidgetTest(QObject *parent = nullptr);
    ~ApplicationsSettingsListWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
