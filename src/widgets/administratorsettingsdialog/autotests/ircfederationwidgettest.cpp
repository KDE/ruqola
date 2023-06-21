/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircfederationwidgettest.h"
#include "administratorsettingsdialog/ircfederation/ircfederationwidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>

QTEST_MAIN(IrcFederationWidgetTest)
IrcFederationWidgetTest::IrcFederationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void IrcFederationWidgetTest::shouldHaveDefaultValues()
{
    IrcFederationWidget w(nullptr);

    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QVERIFY(!mEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("IRC_Enabled"));

    auto mProtocol = w.findChild<QComboBox *>(QStringLiteral("mProtocol"));
    QVERIFY(mProtocol);
    QCOMPARE(mProtocol->count(), 1);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProtocol), QStringLiteral("IRC_Protocol"));

    auto mHost = w.findChild<QLineEdit *>(QStringLiteral("mHost"));
    QVERIFY(mHost);
    QVERIFY(mHost->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHost), QStringLiteral("IRC_Host"));

    auto mPort = w.findChild<QSpinBox *>(QStringLiteral("mPort"));
    QVERIFY(mPort);
    QVERIFY(!mPort->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPort), QStringLiteral("IRC_Port"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("IRC_Port"));

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mName), QStringLiteral("IRC_Name"));

    auto mDescription = w.findChild<QLineEdit *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);
    QVERIFY(mDescription->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDescription), QStringLiteral("IRC_Description"));

    auto mLocalPassword = w.findChild<QLineEdit *>(QStringLiteral("mLocalPassword"));
    QVERIFY(mLocalPassword);
    QVERIFY(mLocalPassword->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLocalPassword), QStringLiteral("IRC_Local_Password"));

    auto mPeerPassword = w.findChild<QLineEdit *>(QStringLiteral("mPeerPassword"));
    QVERIFY(mPeerPassword);
    QVERIFY(mPeerPassword->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPeerPassword), QStringLiteral("IRC_Peer_Password"));
}

#include "moc_ircfederationwidgettest.cpp"
