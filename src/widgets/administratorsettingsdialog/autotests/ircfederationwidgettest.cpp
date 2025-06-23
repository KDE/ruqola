/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircfederationwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/ircfederation/ircfederationwidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

QTEST_MAIN(IrcFederationWidgetTest)
IrcFederationWidgetTest::IrcFederationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void IrcFederationWidgetTest::shouldHaveDefaultValues()
{
    IrcFederationWidget w(nullptr);

    auto mEnabled = w.findChild<QCheckBox *>(u"mEnabled"_s);
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QVERIFY(!mEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), u"IRC_Enabled"_s);

    auto mProtocol = w.findChild<QComboBox *>(u"mProtocol"_s);
    QVERIFY(mProtocol);
    QCOMPARE(mProtocol->count(), 1);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProtocol), u"IRC_Protocol"_s);

    auto mHost = w.findChild<QLineEdit *>(u"mHost"_s);
    QVERIFY(mHost);
    QVERIFY(mHost->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHost), u"IRC_Host"_s);

    auto mPort = w.findChild<QSpinBox *>(u"mPort"_s);
    QVERIFY(mPort);
    QVERIFY(!mPort->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPort), u"IRC_Port"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"IRC_Port"_s);

    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mName), u"IRC_Name"_s);

    auto mDescription = w.findChild<QLineEdit *>(u"mDescription"_s);
    QVERIFY(mDescription);
    QVERIFY(mDescription->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDescription), u"IRC_Description"_s);

    auto mLocalPassword = w.findChild<QLineEdit *>(u"mLocalPassword"_s);
    QVERIFY(mLocalPassword);
    QVERIFY(mLocalPassword->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLocalPassword), u"IRC_Local_Password"_s);

    auto mPeerPassword = w.findChild<QLineEdit *>(u"mPeerPassword"_s);
    QVERIFY(mPeerPassword);
    QVERIFY(mPeerPassword->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPeerPassword), u"IRC_Peer_Password"_s);
}

#include "moc_ircfederationwidgettest.cpp"
