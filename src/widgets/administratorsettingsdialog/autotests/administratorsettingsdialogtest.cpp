/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingsdialogtest.h"
#include "administratorsettingsdialog/administratorsettingsdialog.h"
#include <QTest>
QTEST_MAIN(AdministratorSettingsDialogTest)

AdministratorSettingsDialogTest::AdministratorSettingsDialogTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorSettingsDialogTest::shouldHaveDefaultValues()
{
    AdministratorSettingsDialog d(nullptr);
    // TODO
}
