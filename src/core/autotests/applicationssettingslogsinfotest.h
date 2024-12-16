/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ApplicationsSettingsLogsInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsLogsInfoTest(QObject *parent = nullptr);
    ~ApplicationsSettingsLogsInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadApplicationLogsInfo();
    void shouldLoadApplicationLogsInfo_data();
};
