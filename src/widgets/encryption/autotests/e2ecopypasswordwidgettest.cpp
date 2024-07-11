/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypasswordwidgettest.h"
#include "encryption/e2ecopypasswordwidget.h"
#include <QLabel>
#include <QTest>
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
}

#include "moc_e2ecopypasswordwidgettest.cpp"
