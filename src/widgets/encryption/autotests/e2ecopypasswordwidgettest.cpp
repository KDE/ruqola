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
E2eCopyPasswordWidgetTest::E2eCopyPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2eCopyPasswordWidgetTest::shouldHaveDefaultValues()
{
    E2eCopyPasswordWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QCOMPARE(label->textFormat(), Qt::RichText);
}

#include "moc_e2ecopypasswordwidgettest.cpp"
