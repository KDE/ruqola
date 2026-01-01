/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mobilesettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/mobile/mobilesettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
QTEST_MAIN(MobileSettingsWidgetTest)
MobileSettingsWidgetTest::MobileSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MobileSettingsWidgetTest::shouldHaveDefaultValues()
{
    MobileSettingsWidget w(nullptr);

    auto mAllowSaveMediaGallery = w.findChild<QCheckBox *>(u"mAllowSaveMediaGallery"_s);
    QVERIFY(mAllowSaveMediaGallery);
    QVERIFY(!mAllowSaveMediaGallery->isChecked());
    QVERIFY(!mAllowSaveMediaGallery->text().isEmpty());
    QVERIFY(mAllowSaveMediaGallery->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowSaveMediaGallery), u"Allow_Save_Media_to_Gallery"_s);

    auto screenLockLabel = w.findChild<QLabel *>(u"screenLockLabel"_s);
    QVERIFY(screenLockLabel);
    QVERIFY(!screenLockLabel->text().isEmpty());

    auto mForceScreenLock = w.findChild<QCheckBox *>(u"mForceScreenLock"_s);
    QVERIFY(mForceScreenLock);
    QVERIFY(!mForceScreenLock->isChecked());
    QVERIFY(!mForceScreenLock->text().isEmpty());
    QVERIFY(!mForceScreenLock->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForceScreenLock), u"Force_Screen_Lock"_s);

    auto mForceScreenLockAfter = w.findChild<QSpinBox *>(u"mForceScreenLockAfter"_s);
    QVERIFY(mForceScreenLockAfter);
    QVERIFY(!mForceScreenLockAfter->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForceScreenLockAfter), u"Force_Screen_Lock_After"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Force_Screen_Lock_After"_s);
}

#include "moc_mobilesettingswidgettest.cpp"
