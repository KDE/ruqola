/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircfederationwidgettest.h"
#include "administratorsettingsdialog/ircfederation/ircfederationwidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
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
}
