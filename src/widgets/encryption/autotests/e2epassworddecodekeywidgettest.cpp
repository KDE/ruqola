/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeywidgettest.h"
#include "encryption/e2epassworddecodekeywidget.h"
#include <KPasswordLineEdit>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(E2ePasswordDecodeKeyWidgetTest)

E2ePasswordDecodeKeyWidgetTest::E2ePasswordDecodeKeyWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2ePasswordDecodeKeyWidgetTest::shouldHaveDefaultValues()
{
    E2ePasswordDecodeKeyWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QVERIFY(label->wordWrap());

    auto mPassword = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPassword"));
    QVERIFY(mPassword);
    QVERIFY(mPassword->password().isEmpty());
}

#include "moc_e2epassworddecodekeywidgettest.cpp"
