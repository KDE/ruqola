/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationreportedmessageinfotest.h"
#include "moderation/moderationreportedmessageinfo.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ModerationReportedMessageInfoTest)
ModerationReportedMessageInfoTest::ModerationReportedMessageInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationReportedMessageInfoTest::shouldHaveDefaultValues()
{
    ModerationReportedMessageInfo i;
    QVERIFY(i.userId().isEmpty());
    QVERIFY(i.name().isEmpty());
    QVERIFY(i.userName().isEmpty());
    QVERIFY(i.msgId().isEmpty());
    QCOMPARE(i.count(), 0);
    QVERIFY(!i.isUserDeleted());
    QVERIFY(i.message().isEmpty());
    QVERIFY(i.roomList().isEmpty());
}

void ModerationReportedMessageInfoTest::shouldModerationInfo()
{
    QFETCH(QString, name);
    QFETCH(ModerationReportedMessageInfo, moderationInfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/moderation/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    ModerationReportedMessageInfo result;
    result.parseModerationInfo(obj);
    const bool equal = result == moderationInfo;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " deviceInfo " << moderationInfo;
    }
    QVERIFY(equal);
}

void ModerationReportedMessageInfoTest::shouldModerationInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ModerationReportedMessageInfo>("moderationInfo");
    QTest::addRow("moderationempty") << QStringLiteral("moderationempty") << ModerationReportedMessageInfo();
    {
        ModerationReportedMessageInfo info;
        info.setUserId("SSSSSQ4iShzD9T2"_ba);
        info.setName(QStringLiteral("Name Laur"));
        info.setUserName(QStringLiteral("laurent"));
        info.setMsgId("XBHe3ocWiDApCc"_ba);
        info.setCount(3);
        info.setIsUserDeleted(false);
        info.setMessage(QStringLiteral("H"));
        qint64 val = 1695363298494;
        info.setCreatedAt(val);
        QStringList list;
        list << QStringLiteral("test1") << QStringLiteral("test-team1");
        info.setRoomList(list);
        QTest::addRow("moderation1") << QStringLiteral("moderation1") << info;
    }
}

#include "moc_moderationreportedmessageinfotest.cpp"
