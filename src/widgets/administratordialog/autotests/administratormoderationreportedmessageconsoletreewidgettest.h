/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AdministratorModerationReportedMessageConsoleTreeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorModerationReportedMessageConsoleTreeWidgetTest(QObject *parent = nullptr);
    ~AdministratorModerationReportedMessageConsoleTreeWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
