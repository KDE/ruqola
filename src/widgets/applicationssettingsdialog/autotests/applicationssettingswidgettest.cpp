/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidgettest.h"
#include "applicationssettingsdialog/applicationssettingsinprogresswidget.h"
#include "applicationssettingsdialog/applicationssettingslistview.h"
#include "applicationssettingsdialog/applicationssettingssearchwidget.h"
#include "applicationssettingsdialog/applicationssettingswidget.h"
#include "applicationssettingsdialog/appscountinfowidget.h"
#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ApplicationsSettingsWidgetTest)
ApplicationsSettingsWidgetTest::ApplicationsSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsWidget d(nullptr);
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mApplicationsSettingsSearchWidget = d.findChild<ApplicationsSettingsSearchWidget *>(QStringLiteral("mApplicationsSettingsSearchWidget"));
    QVERIFY(mApplicationsSettingsSearchWidget);
    auto mApplicationsSettingsListView = d.findChild<ApplicationsSettingsListView *>(QStringLiteral("mApplicationsSettingsListView"));
    QVERIFY(mApplicationsSettingsListView);
    auto mApplicationsSettingsInProgressWidget = d.findChild<ApplicationsSettingsInProgressWidget *>(QStringLiteral("mApplicationsSettingsInProgressWidget"));
    QVERIFY(mApplicationsSettingsInProgressWidget);
    auto mStackedWidget = d.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
    auto mWidgetListView = d.findChild<QWidget *>(QStringLiteral("mWidgetListView"));
    QVERIFY(mWidgetListView);
    auto mAppsCountInfoWidget = d.findChild<AppsCountInfoWidget *>("mAppsCountInfoWidget");
    QVERIFY(mAppsCountInfoWidget);
}

#include "moc_applicationssettingswidgettest.cpp"
