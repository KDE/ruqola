/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AdministratorUsersWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorUsersWidgetTest(QObject *parent = nullptr);
    ~AdministratorUsersWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

