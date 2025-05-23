/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class LDapSettingsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit LDapSettingsWidgetTest(QObject *parent = nullptr);
    ~LDapSettingsWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
