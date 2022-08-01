/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ldapsettingswidgettest.h"
#include "administratorsettingsdialog/ldap/ldapsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>

QTEST_MAIN(LDapSettingsWidgetTest)
LDapSettingsWidgetTest::LDapSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LDapSettingsWidgetTest::shouldHaveDefaultValues()
{
    LDapSettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QVERIFY(!mEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("LDAP_Enable"));

    auto mHost = w.findChild<QLineEdit *>(QStringLiteral("mHost"));
    QVERIFY(mHost);
    QVERIFY(mHost->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHost), QStringLiteral("LDAP_Host"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("LDAP_Host"));
}
