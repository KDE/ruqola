/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "readreceipttest.h"
#include "readreceipts/readreceipt.h"
#include <QTest>
QTEST_GUILESS_MAIN(ReadReceiptTest)

ReadReceiptTest::ReadReceiptTest(QObject *parent)
    : QObject{parent}
{
}

void ReadReceiptTest::shouldHaveDefaultValues()
{
    const ReadReceipt w;
    QVERIFY(w.userId().isEmpty());
    QCOMPARE(w.timeStamp(), -1);
    QCOMPARE(w.updatedAt(), -1);
    // TODO
}
#include "moc_readreceipttest.cpp"
