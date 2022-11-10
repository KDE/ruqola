/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidgettest.h"
#include "administratorsettingsdialog/troubleshoot/troubleshootsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QTest>

QTEST_MAIN(TroubleshootSettingsWidgetTest)
TroubleshootSettingsWidgetTest::TroubleshootSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TroubleshootSettingsWidgetTest::shouldHaveDefaultValues()
{
    TroubleshootSettingsWidget w(nullptr);
    auto mDisableNotifications = w.findChild<QCheckBox *>(QStringLiteral("mDisableNotifications"));
    QVERIFY(mDisableNotifications);
    QVERIFY(!mDisableNotifications->isChecked());
    QVERIFY(!mDisableNotifications->text().isEmpty());
    QVERIFY(mDisableNotifications->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableNotifications), QStringLiteral("Troubleshoot_Disable_Notifications"));
}
