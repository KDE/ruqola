/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "layoutsettingswidgettest.h"
#include "administratorsettingsdialog/layout/layoutsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(LayoutSettingsWidgetTest)

LayoutSettingsWidgetTest::LayoutSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LayoutSettingsWidgetTest::shouldHaveDefaultValues()
{
    LayoutSettingsWidget w(nullptr);
    auto mDisplayRoles = w.findChild<QCheckBox *>(QStringLiteral("mDisplayRoles"));
    QVERIFY(mDisplayRoles);
    QVERIFY(!mDisplayRoles->isChecked());
    QVERIFY(!mDisplayRoles->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisplayRoles), QStringLiteral("UI_DisplayRoles"));

    auto mUseRealName = w.findChild<QCheckBox *>(QStringLiteral("mDisplayRoles"));
    QVERIFY(mUseRealName);
    QVERIFY(!mUseRealName->isChecked());
    QVERIFY(!mUseRealName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseRealName), QStringLiteral("UI_Use_Real_Name"));
}
