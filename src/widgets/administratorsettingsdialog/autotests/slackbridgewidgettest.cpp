/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidgettest.h"
#include "administratorsettingsdialog/slackbridge/slackbridgewidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QTest>
QTEST_MAIN(SlackBridgeWidgetTest)
SlackBridgeWidgetTest::SlackBridgeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void SlackBridgeWidgetTest::shouldHaveDefaultValues()
{
    SlackBridgeWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QVERIFY(!mEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("SlackBridge_Enabled"));
}
