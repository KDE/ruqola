/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsdescriptiontabwidgettest.h"
#include "applicationssettingsdialog/applicationssettingsdescriptiontabwidget.h"
#include "applicationssettingsdialog/applicationssettingsdescriptionwidget.h"
#include "applicationssettingsdialog/applicationssettingslogswidget.h"
#include "applicationssettingsdialog/applicationssettingsversionwidget.h"
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ApplicationsSettingsDescriptionTabWidgetTest)

ApplicationsSettingsDescriptionTabWidgetTest::ApplicationsSettingsDescriptionTabWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsDescriptionTabWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsDescriptionTabWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(QStringLiteral("mTabWidget"));
    QVERIFY(mTabWidget);

    auto mDescriptionWidget = mTabWidget->findChild<ApplicationsSettingsDescriptionWidget *>(QStringLiteral("mDescriptionWidget"));
    QVERIFY(mDescriptionWidget);

    auto mVersionWidget = mTabWidget->findChild<ApplicationsSettingsVersionWidget *>(QStringLiteral("mVersionWidget"));
    QVERIFY(mVersionWidget);

    auto mLogsWidget = mTabWidget->findChild<ApplicationsSettingsLogsWidget *>(QStringLiteral("mLogsWidget"));
    QVERIFY(mLogsWidget);
}

#include "moc_applicationssettingsdescriptiontabwidgettest.cpp"
