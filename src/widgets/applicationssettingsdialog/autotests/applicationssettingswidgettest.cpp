/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mApplicationsSettingsSearchWidget = d.findChild<ApplicationsSettingsSearchWidget *>(u"mApplicationsSettingsSearchWidget"_s);
    QVERIFY(mApplicationsSettingsSearchWidget);
    auto mApplicationsSettingsListView = d.findChild<ApplicationsSettingsListView *>(u"mApplicationsSettingsListView"_s);
    QVERIFY(mApplicationsSettingsListView);
    auto mApplicationsSettingsInProgressWidget = d.findChild<ApplicationsSettingsInProgressWidget *>(u"mApplicationsSettingsInProgressWidget"_s);
    QVERIFY(mApplicationsSettingsInProgressWidget);
    auto mStackedWidget = d.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);
    auto mWidgetListView = d.findChild<QWidget *>(u"mWidgetListView"_s);
    QVERIFY(mWidgetListView);
    auto mAppsCountInfoWidget = d.findChild<AppsCountInfoWidget *>("mAppsCountInfoWidget");
    QVERIFY(mAppsCountInfoWidget);
}

#include "moc_applicationssettingswidgettest.cpp"
