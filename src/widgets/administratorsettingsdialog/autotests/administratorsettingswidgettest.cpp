/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidgettest.h"
#include "administratorsettingsdialog/administratorsettingswidget.h"
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorSettingsWidgetTest)

AdministratorSettingsWidgetTest::AdministratorSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorSettingsWidgetTest::shouldHaveDefaultValues()
{
    AdministratorSettingsWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(QStringLiteral("mTabWidget"));
    QVERIFY(mTabWidget);
}
