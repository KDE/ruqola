/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "readreceipttest.h"
#include "readreceipts/readreceipt.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(ReadReceiptTest)
using namespace Qt::Literals::StringLiterals;
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
    QVERIFY(!w.isValid());
}

void ReadReceiptTest::shouldLoadReadReceipt_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ReadReceipt>("readReceipt");

    {
        ReadReceipt p;
        p.setUserId("9zqF4Dpv5GJJgppWM"_ba);
        p.setUpdatedAt(1781013434870);
        p.setTimeStamp(1781013434870);
        QTest::addRow("readreceipt1") << u"readreceipt1"_s << p;
    }
}

void ReadReceiptTest::shouldLoadReadReceipt()
{
    QFETCH(QString, name);
    QFETCH(ReadReceipt, readReceipt);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/readreceipt/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    ReadReceipt r;
    r.parseReadReceiptInfo(obj);
    // qDebug() << " r " << r << " permission " << permission;
    QCOMPARE(r, readReceipt);
}
#include "moc_readreceipttest.cpp"
