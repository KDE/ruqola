/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypasswordwidgettest.h"
#include "encryption/e2ecopypasswordwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(E2eCopyPasswordWidgetTest)
using namespace Qt::Literals::StringLiterals;
E2eCopyPasswordWidgetTest::E2eCopyPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2eCopyPasswordWidgetTest::shouldHaveDefaultValues()
{
    E2eCopyPasswordWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>("mainLayout"_L1);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>("label"_L1);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QCOMPARE(label->textFormat(), Qt::RichText);

    auto passwordLabel = w.findChild<QLabel *>("passwordLabel"_L1);
    QVERIFY(passwordLabel);
    QCOMPARE(passwordLabel->textFormat(), Qt::RichText);
    QCOMPARE(passwordLabel->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto passwordLayout = w.findChild<QHBoxLayout *>("passwordLayout"_L1);
    QCOMPARE(passwordLayout->contentsMargins(), QMargins{});

    auto copyToolButton = w.findChild<QToolButton *>("copyToolButton"_L1);
    QVERIFY(copyToolButton);
    QVERIFY(copyToolButton->autoRaise());
    QVERIFY(!copyToolButton->toolTip().isEmpty());
}

#include "moc_e2ecopypasswordwidgettest.cpp"
