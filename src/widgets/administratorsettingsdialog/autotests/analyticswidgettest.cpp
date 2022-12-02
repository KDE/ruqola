/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "analyticswidgettest.h"
#include "administratorsettingsdialog/analytics/analyticswidget.h"
#include "settingswidgetshelper.h"
#include <QTest>
QTEST_MAIN(AnalyticsWidgetTest)
AnalyticsWidgetTest::AnalyticsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AnalyticsWidgetTest::shouldHaveDefaultValues()
{
    AnalyticsWidget w(nullptr);

    auto mMessages = w.findChild<QCheckBox *>(QStringLiteral("mMessages"));
    QVERIFY(mMessages);
    QVERIFY(!mMessages->isChecked());
    QVERIFY(!mMessages->text().isEmpty());
    QVERIFY(mMessages->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMessages), QStringLiteral("Analytics_features_messages"));
}
