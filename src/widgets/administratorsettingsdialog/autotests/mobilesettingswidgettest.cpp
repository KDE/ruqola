/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mobilesettingswidgettest.h"
#include "administratorsettingsdialog/mobile/mobilesettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QTest>
QTEST_MAIN(MobileSettingsWidgetTest)
MobileSettingsWidgetTest::MobileSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MobileSettingsWidgetTest::shouldHaveDefaultValues()
{
    MobileSettingsWidget w(nullptr);

    auto mAllowSaveMediaGallery = w.findChild<QCheckBox *>(QStringLiteral("mAllowSaveMediaGallery"));
    QVERIFY(mAllowSaveMediaGallery);
    QVERIFY(!mAllowSaveMediaGallery->isChecked());
    QVERIFY(!mAllowSaveMediaGallery->text().isEmpty());
    QVERIFY(mAllowSaveMediaGallery->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowSaveMediaGallery), QStringLiteral("Allow_Save_Media_to_Gallery"));

    auto screenLockLabel = w.findChild<QLabel *>(QStringLiteral("screenLockLabel"));
    QVERIFY(screenLockLabel);
    QVERIFY(!screenLockLabel->text().isEmpty());

    auto mForceScreenLock = w.findChild<QCheckBox *>(QStringLiteral("mForceScreenLock"));
    QVERIFY(mForceScreenLock);
    QVERIFY(!mForceScreenLock->isChecked());
    QVERIFY(!mForceScreenLock->text().isEmpty());
    QVERIFY(!mForceScreenLock->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForceScreenLock), QStringLiteral("Force_Screen_Lock"));

    auto mForceScreenLockAfter = w.findChild<QSpinBox *>(QStringLiteral("mForceScreenLockAfter"));
    QVERIFY(mForceScreenLockAfter);
    QVERIFY(!mForceScreenLockAfter->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForceScreenLockAfter), QStringLiteral("SMTP_Port"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Port"));
}
