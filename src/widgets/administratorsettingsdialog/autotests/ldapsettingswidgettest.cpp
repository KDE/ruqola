/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ldapsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/ldap/ldapsettingswidget.h"
#include "settingswidgetshelper.h"
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>

QTEST_MAIN(LDapSettingsWidgetTest)
LDapSettingsWidgetTest::LDapSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LDapSettingsWidgetTest::shouldHaveDefaultValues()
{
    LDapSettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(u"mEnabled"_s);
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QVERIFY(!mEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), u"LDAP_Enable"_s);

    auto mHost = w.findChild<QLineEdit *>(u"mHost"_s);
    QVERIFY(mHost);
    QVERIFY(mHost->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHost), u"LDAP_Host"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"LDAP_Host"_s);

    auto mPort = w.findChild<QSpinBox *>(u"mPort"_s);
    QVERIFY(mPort);
    QVERIFY(!mPort->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPort), u"LDAP_Port"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"LDAP_Port"_s);

    auto mReconnect = w.findChild<QCheckBox *>(u"mReconnect"_s);
    QVERIFY(mReconnect);
    QVERIFY(!mReconnect->isChecked());
    QVERIFY(!mReconnect->text().isEmpty());
    QVERIFY(!mReconnect->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mReconnect), u"LDAP_Reconnect"_s);

    auto mAuthenticationEnabled = w.findChild<QCheckBox *>(u"mAuthenticationEnabled"_s);
    QVERIFY(mAuthenticationEnabled);
    QVERIFY(!mAuthenticationEnabled->isChecked());
    QVERIFY(!mAuthenticationEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAuthenticationEnabled), u"LDAP_Authentication"_s);

    auto authenticationLabel = w.findChild<QLabel *>(u"authenticationLabel"_s);
    QVERIFY(authenticationLabel);
    QVERIFY(!authenticationLabel->text().isEmpty());

    auto mAuthenticationUserDN = w.findChild<QLineEdit *>(u"mAuthenticationUserDN"_s);
    QVERIFY(mAuthenticationUserDN);
    QVERIFY(mAuthenticationUserDN->text().isEmpty());
    QVERIFY(!mAuthenticationUserDN->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAuthenticationUserDN), u"LDAP_Authentication_UserDN"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"LDAP_Authentication_UserDN"_s);

    auto timeoutLabel = w.findChild<QLabel *>(u"timeoutLabel"_s);
    QVERIFY(timeoutLabel);
    QVERIFY(!timeoutLabel->text().isEmpty());

    auto mTimeouts = w.findChild<QSpinBox *>(u"mTimeouts"_s);
    QVERIFY(mTimeouts);
    QVERIFY(!mTimeouts->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTimeouts), u"LDAP_Timeout"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"LDAP_Timeout"_s);

    auto mConnectionTimeouts = w.findChild<QSpinBox *>(u"mConnectionTimeouts"_s);
    QVERIFY(mConnectionTimeouts);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mConnectionTimeouts), u"LDAP_Connect_Timeout"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"LDAP_Connect_Timeout"_s);

    auto mIdleTimeouts = w.findChild<QSpinBox *>(u"mIdleTimeouts"_s);
    QVERIFY(mIdleTimeouts);
    QVERIFY(!mIdleTimeouts->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mIdleTimeouts), u"LDAP_Idle_Timeout"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"LDAP_Idle_Timeout"_s);

    auto mAuthenticationPassword = w.findChild<KPasswordLineEdit *>(u"mAuthenticationPassword"_s);
    QVERIFY(mAuthenticationPassword);
    QVERIFY(mAuthenticationPassword->password().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAuthenticationPassword), u"LDAP_Authentication_Password"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"LDAP_Authentication_Password"_s);
}

#include "moc_ldapsettingswidgettest.cpp"
