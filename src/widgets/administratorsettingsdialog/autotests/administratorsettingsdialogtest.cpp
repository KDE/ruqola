/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingsdialogtest.h"
#include "administratorsettingsdialog/administratorsettingsdialog.h"
#include "administratorsettingsdialog/administratorsettingswidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorSettingsDialogTest)

AdministratorSettingsDialogTest::AdministratorSettingsDialogTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorSettingsDialogTest::shouldHaveDefaultValues()
{
    AdministratorSettingsDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAdministratorSettingsWidget = d.findChild<AdministratorSettingsWidget *>(QStringLiteral("mAdministratorSettingsWidget"));
    QVERIFY(mAdministratorSettingsWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
