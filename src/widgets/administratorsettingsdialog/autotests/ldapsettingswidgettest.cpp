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

    auto mAuthenticationEnabled = w.findChild<QCheckBox *>(QStringLiteral("mAuthenticationEnabled"));
    QVERIFY(mAuthenticationEnabled);
    QVERIFY(!mAuthenticationEnabled->isChecked());
    QVERIFY(!mAuthenticationEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAuthenticationEnabled), QStringLiteral("LDAP_Authentication"));

    auto authenticationLabel = w.findChild<QLabel *>(QStringLiteral("authenticationLabel"));
    QVERIFY(authenticationLabel);
    QVERIFY(!authenticationLabel->text().isEmpty());

    auto mAuthenticationUserDN = w.findChild<QLineEdit *>(QStringLiteral("mAuthenticationUserDN"));
    QVERIFY(mAuthenticationUserDN);
    QVERIFY(mAuthenticationUserDN->text().isEmpty());
    QVERIFY(!mAuthenticationUserDN->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAuthenticationUserDN), QStringLiteral("LDAP_Authentication_UserDN"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("LDAP_Authentication_UserDN"));

    auto timeoutLabel = w.findChild<QLabel *>(QStringLiteral("timeoutLabel"));
    QVERIFY(timeoutLabel);
    QVERIFY(!timeoutLabel->text().isEmpty());

    auto mTimeouts = w.findChild<QSpinBox *>(QStringLiteral("mTimeouts"));
    QVERIFY(mTimeouts);
    QVERIFY(!mTimeouts->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTimeouts), QStringLiteral("LDAP_Timeout"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("LDAP_Timeout"));

    auto mConnectionTimeouts = w.findChild<QSpinBox *>(QStringLiteral("mConnectionTimeouts"));
    QVERIFY(mConnectionTimeouts);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mConnectionTimeouts), QStringLiteral("LDAP_Connect_Timeout"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("LDAP_Connect_Timeout"));

    auto mIdleTimeouts = w.findChild<QSpinBox *>(QStringLiteral("mIdleTimeouts"));
    QVERIFY(mIdleTimeouts);
    QVERIFY(!mIdleTimeouts->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mIdleTimeouts), QStringLiteral("LDAP_Idle_Timeout"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("LDAP_Idle_Timeout"));
}
