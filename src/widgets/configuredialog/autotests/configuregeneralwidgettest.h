/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ConfigureGeneralWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureGeneralWidgetTest(QObject *parent = nullptr);
    ~ConfigureGeneralWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
