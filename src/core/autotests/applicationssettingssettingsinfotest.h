/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ApplicationsSettingsSettingsInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsSettingsInfoTest(QObject *parent = nullptr);
    ~ApplicationsSettingsSettingsInfoTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadApplicationSettingsInfo();
    void shouldLoadApplicationSettingsInfo_data();
};
