/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationinfotest.h"
#include "moderation/moderationinfo.h"
#include "ruqola_autotest_helper.h"

QTEST_GUILESS_MAIN(ModerationInfoTest)
ModerationInfoTest::ModerationInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationInfoTest::shouldHaveDefaultValues()
{
    ModerationInfo i;
    QVERIFY(i.userId().isEmpty());
    QVERIFY(i.name().isEmpty());
    QVERIFY(i.userName().isEmpty());
    QVERIFY(i.msgId().isEmpty());
    QCOMPARE(i.count(), 0);
    QVERIFY(!i.isUserDeleted());
    QVERIFY(i.message().isEmpty());
    QVERIFY(i.roomList().isEmpty());
}

void ModerationInfoTest::shouldModerationInfo()
{
    QFETCH(QString, name);
    QFETCH(ModerationInfo, moderationInfo);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/moderation/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    ModerationInfo result;
    result.parseModerationInfo(obj);
    const bool equal = result == moderationInfo;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " deviceInfo " << moderationInfo;
    }
    QVERIFY(equal);
}

void ModerationInfoTest::shouldModerationInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ModerationInfo>("moderationInfo");
    QTest::addRow("moderationempty") << QStringLiteral("moderationempty") << ModerationInfo();
    {
        ModerationInfo info;
        info.setUserId(QStringLiteral("SSSSSQ4iShzD9T2"));
        info.setName(QStringLiteral("Name Laur"));
        info.setUserName(QStringLiteral("laurent"));
        info.setMsgId(QStringLiteral("XBHe3ocWiDApCc"));
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

#include "moc_moderationinfotest.cpp"
