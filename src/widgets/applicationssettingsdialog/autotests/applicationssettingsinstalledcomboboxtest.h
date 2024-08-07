/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ApplicationsSettingsInstalledComboBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsInstalledComboBoxTest(QObject *parent = nullptr);
    ~ApplicationsSettingsInstalledComboBoxTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
