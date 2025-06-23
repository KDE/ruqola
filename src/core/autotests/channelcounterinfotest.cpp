/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelcounterinfotest.h"
#include "channelcounterinfo.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ChannelCounterInfoTest)
ChannelCounterInfoTest::ChannelCounterInfoTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelCounterInfoTest::shouldHaveDefaultValues()
{
    ChannelCounterInfo info;
    QCOMPARE(info.unreadMessages(), 0);
    QCOMPARE(info.messageCount(), 0);
    QVERIFY(!info.joined());
    QVERIFY(!info.isValid());
}

void ChannelCounterInfoTest::shouldLoadChannelCounter_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ChannelCounterInfo *>("channelcounter");
    ChannelCounterInfo *result = new ChannelCounterInfo;
    result->setMessageCount(48013);
    result->setUnreadMessages(0);
    result->setJoined(true);
    QDateTime t;
    t.setTimeZone(QTimeZone::UTC);
    t.setDate(QDate(2020, 9, 25));
    t.setTime(QTime(11, 36, 30, 262));
    result->setUnreadFrom(t);

    // TODO
    QTest::addRow("test1") << u"test1"_s << result;
}

void ChannelCounterInfoTest::shouldLoadChannelCounter()
{
    QFETCH(QString, name);
    QFETCH(ChannelCounterInfo *, channelcounter);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/channelcounter/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    ChannelCounterInfo r;
    r.parseCounterInfo(obj);
    const bool equalOwner = (r == *channelcounter);
    if (!equalOwner) {
        qDebug() << "ACTUAL " << r;
        qDebug() << "EXPECTED " << *channelcounter;
    }
    QVERIFY(equalOwner);
}

#include "moc_channelcounterinfotest.cpp"
