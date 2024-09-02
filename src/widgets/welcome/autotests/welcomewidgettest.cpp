/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "welcomewidgettest.h"
#include "welcome/welcomewidget.h"
#include <QHBoxLayout>
#include <QPushButton>
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

    auto addAccountButton = w.findChild<QPushButton *>("addAccountButton"_L1);
    QVERIFY(addAccountButton);
    const QSizePolicy p = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QCOMPARE(addAccountButton->sizePolicy(), p);
    QVERIFY(!addAccountButton->text().isEmpty());
}

#include "moc_welcomewidgettest.cpp"
