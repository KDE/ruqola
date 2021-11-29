/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AdministratorDirectRoomsEditWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorDirectRoomsEditWidgetTest(QObject *parent = nullptr);
    ~AdministratorDirectRoomsEditWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
