/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "welcomewidgettest.h"
#include "welcome/welcomewidget.h"
#include <QHBoxLayout>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(WelcomeWidgetTest)
WelcomeWidgetTest::WelcomeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WelcomeWidgetTest::shouldHaveDefaultValues()
{
    WelcomeWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>("mainLayout"_L1);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    // TODO
}

#include "moc_welcomewidgettest.cpp"
