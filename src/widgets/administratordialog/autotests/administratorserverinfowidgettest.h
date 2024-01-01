/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AdministratorServerInfoWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorServerInfoWidgetTest(QObject *parent = nullptr);
    ~AdministratorServerInfoWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
