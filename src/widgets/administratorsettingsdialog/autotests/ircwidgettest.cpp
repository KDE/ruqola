/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/irc/ircwidget.h"
#include "settingswidgetshelper.h"
#include <QLineEdit>
#include <QSpinBox>

QTEST_MAIN(IrcWidgetTest)

IrcWidgetTest::IrcWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void IrcWidgetTest::shouldHaveDefaultValues()
{
    IrcWidget w(nullptr);
    auto mCacheLimitOutBoundMessage = w.findChild<QSpinBox *>(u"mCacheLimitOutBoundMessage"_s);
    QVERIFY(mCacheLimitOutBoundMessage);
    QVERIFY(!mCacheLimitOutBoundMessage->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mCacheLimitOutBoundMessage), u"IRC_Message_Cache_Size"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"IRC_Message_Cache_Size"_s);

    auto mLoginSuccessful = w.findChild<QLineEdit *>(u"mLoginSuccessful"_s);
    QVERIFY(mLoginSuccessful);
    QVERIFY(mLoginSuccessful->text().isEmpty());
    QVERIFY(!mLoginSuccessful->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLoginSuccessful), u"IRC_RegEx_successLogin"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"IRC_RegEx_successLogin"_s);
    // TODO
}

#include "moc_ircwidgettest.cpp"
