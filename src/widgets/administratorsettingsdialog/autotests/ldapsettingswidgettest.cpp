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

    auto mPort = w.findChild<QSpinBox *>(QStringLiteral("mPort"));
    QVERIFY(mPort);
    QVERIFY(!mPort->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPort), QStringLiteral("LDAP_Port"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("LDAP_Port"));

    auto mReconnect = w.findChild<QCheckBox *>(QStringLiteral("mReconnect"));
    QVERIFY(mReconnect);
    QVERIFY(!mReconnect->isChecked());
    QVERIFY(!mReconnect->text().isEmpty());
    QVERIFY(!mReconnect->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mReconnect), QStringLiteral("LDAP_Reconnect"));
}
