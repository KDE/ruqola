/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircwidgettest.h"
#include "administratorsettingsdialog/irc/ircwidget.h"
#include "settingswidgetshelper.h"
#include <QSpinBox>
#include <QTest>

QTEST_MAIN(IrcWidgetTest)

IrcWidgetTest::IrcWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void IrcWidgetTest::shouldHaveDefaultValues()
{
    IrcWidget w(nullptr);
    auto mCacheLimitOutBoundMessage = w.findChild<QSpinBox *>(QStringLiteral("mCacheLimitOutBoundMessage"));
    QVERIFY(mCacheLimitOutBoundMessage);
    QVERIFY(!mCacheLimitOutBoundMessage->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mCacheLimitOutBoundMessage), QStringLiteral("IRC_Message_Cache_Size"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("IRC_Message_Cache_Size"));

    // TODO
}
