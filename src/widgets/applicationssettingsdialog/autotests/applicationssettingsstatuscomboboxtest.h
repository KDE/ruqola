/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ApplicationsSettingsStatusComboBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsStatusComboBoxTest(QObject *parent = nullptr);
    ~ApplicationsSettingsStatusComboBoxTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
