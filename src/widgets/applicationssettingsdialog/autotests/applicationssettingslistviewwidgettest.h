/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ApplicationsSettingsListViewWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListViewWidgetTest(QObject *parent = nullptr);
    ~ApplicationsSettingsListViewWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
};
