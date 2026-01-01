/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsdescriptiontabwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationssettingsdialog/applicationssettingsdescriptiontabwidget.h"
#include "applicationssettingsdialog/applicationssettingsdescriptionwidget.h"
#include "applicationssettingsdialog/applicationssettingslogswidget.h"
#include "applicationssettingsdialog/applicationssettingssettingswidget.h"
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
    ApplicationsSettingsDescriptionTabWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(u"mTabWidget"_s);
    QVERIFY(mTabWidget);

    auto mDescriptionWidget = mTabWidget->findChild<ApplicationsSettingsDescriptionWidget *>(u"mDescriptionWidget"_s);
    QVERIFY(mDescriptionWidget);

    auto mVersionWidget = mTabWidget->findChild<ApplicationsSettingsVersionWidget *>(u"mVersionWidget"_s);
    QVERIFY(mVersionWidget);

    auto mLogsWidget = mTabWidget->findChild<ApplicationsSettingsLogsWidget *>(u"mLogsWidget"_s);
    QVERIFY(mLogsWidget);

    auto mSettingsWidget = mTabWidget->findChild<ApplicationsSettingsSettingsWidget *>(u"mSettingsWidget"_s);
    QVERIFY(mSettingsWidget);
}

#include "moc_applicationssettingsdescriptiontabwidgettest.cpp"
