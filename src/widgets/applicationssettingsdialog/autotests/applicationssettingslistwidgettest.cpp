/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingslistwidgettest.h"
#include "applicationssettingsdialog/applicationssettingslistview.h"
#include "applicationssettingsdialog/applicationssettingslistwidget.h"
#include "applicationssettingsdialog/applicationssettingssearchwidget.h"
#include "applicationssettingsdialog/appscountinfowidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ApplicationsSettingsListWidgetTest)
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsListWidgetTest::ApplicationsSettingsListWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsListWidgetTest::shouldHaveDefaultValues()
{
    const ApplicationsSettingsListWidget w(nullptr);

    auto mApplicationsSettingsSearchWidget = w.findChild<ApplicationsSettingsSearchWidget *>(u"mApplicationsSettingsSearchWidget"_s);
    QVERIFY(mApplicationsSettingsSearchWidget);

    auto mApplicationsSettingsListView = w.findChild<ApplicationsSettingsListView *>(u"mApplicationsSettingsListView"_s);
    QVERIFY(mApplicationsSettingsListView);

    auto mAppsCountInfoWidget = w.findChild<AppsCountInfoWidget *>(u"mAppsCountInfoWidget"_s);
    QVERIFY(mAppsCountInfoWidget);

    auto mainLayout = w.findChild<QVBoxLayout *>("mainLayout"_L1);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    // TODO
}
