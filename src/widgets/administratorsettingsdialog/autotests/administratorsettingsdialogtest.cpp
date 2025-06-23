/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingsdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/administratorsettingsdialog.h"
#include "administratorsettingsdialog/administratorsettingswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorSettingsDialogTest)

AdministratorSettingsDialogTest::AdministratorSettingsDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorSettingsDialogTest::shouldHaveDefaultValues()
{
    AdministratorSettingsDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mAdministratorSettingsWidget = d.findChild<AdministratorSettingsWidget *>(u"mAdministratorSettingsWidget"_s);
    QVERIFY(mAdministratorSettingsWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_administratorsettingsdialogtest.cpp"
