/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AdministratorRoomsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorRoomsWidgetTest(QObject *parent = nullptr);
    ~AdministratorRoomsWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
