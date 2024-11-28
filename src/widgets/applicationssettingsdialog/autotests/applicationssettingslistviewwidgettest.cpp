/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingslistviewwidgettest.h"
#include "applicationssettingsdialog/applicationssettingslistview.h"
#include "applicationssettingsdialog/applicationssettingslistviewwidget.h"
#include "applicationssettingsdialog/appscountinfowidget.h"
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ApplicationsSettingsListViewWidgetTest)
ApplicationsSettingsListViewWidgetTest::ApplicationsSettingsListViewWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsListViewWidgetTest::shouldHaveDefaultValue()
{
    ApplicationsSettingsListViewWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>("mainLayout"_L1);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAppsCountInfoWidget = w.findChild<AppsCountInfoWidget *>("mAppsCountInfoWidget"_L1);
    QVERIFY(mAppsCountInfoWidget);

    auto mApplicationsSettingsListView = w.findChild<ApplicationsSettingsListView *>("mApplicationsSettingsListView"_L1);
    QVERIFY(mApplicationsSettingsListView);
}

#include "moc_applicationssettingslistviewwidgettest.cpp"
