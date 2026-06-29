/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "readreceiptstest.h"
#include "readreceipts/readreceipts.h"
#include <QTest>
QTEST_GUILESS_MAIN(ReadReceiptsTest)

ReadReceiptsTest::ReadReceiptsTest(QObject *parent)
    : QObject{parent}
{
}

void ReadReceiptsTest::shouldHaveDefaultValues()
{
    const ReadReceipts w;
    QVERIFY(w.isEmpty());
    QCOMPARE(w.count(), 0);
    // TODO
}

#include "moc_readreceiptstest.cpp"
