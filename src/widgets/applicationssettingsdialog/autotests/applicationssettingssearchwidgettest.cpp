/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingssearchwidgettest.h"
#include "applicationssettingsdialog/applicationssettingssearchwidget.h"
#include <QHBoxLayout>
#include <QTest>
QTEST_MAIN(ApplicationsSettingsSearchWidgetTest)
ApplicationsSettingsSearchWidgetTest::ApplicationsSettingsSearchWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsSearchWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsSearchWidget d;
    auto mainLayout = d.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_applicationssettingssearchwidgettest.cpp"
