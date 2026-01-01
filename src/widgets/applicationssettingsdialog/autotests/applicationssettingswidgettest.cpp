/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidgettest.h"

#include "applicationssettingsdialog/applicationssettingsinprogresswidget.h"
#include "applicationssettingsdialog/applicationssettingslistwidget.h"
#include "applicationssettingsdialog/applicationssettingswidget.h"
#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ApplicationsSettingsWidgetTest)
using namespace Qt::Literals::StringLiterals;
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

    auto mApplicationsSettingsInProgressWidget = d.findChild<ApplicationsSettingsInProgressWidget *>(u"mApplicationsSettingsInProgressWidget"_s);
    QVERIFY(mApplicationsSettingsInProgressWidget);
    auto mStackedWidget = d.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);
    auto mApplicationsSettingsListWidget = d.findChild<ApplicationsSettingsListWidget *>(u"mApplicationsSettingsListWidget"_s);
    QVERIFY(mApplicationsSettingsListWidget);
}

#include "moc_applicationssettingswidgettest.cpp"
