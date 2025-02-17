/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfotest.h"
#include "moderation/moderationreportinfo.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ModerationReportInfoTest)
ModerationReportInfoTest::ModerationReportInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationReportInfoTest::shouldHaveDefaultValues()
{
    ModerationReportInfo info;
    QVERIFY(info.reportIdentifier().isEmpty());
    QVERIFY(info.description().isEmpty());
    QCOMPARE(info.timeStamp(), -1);
    QVERIFY(info.roomId().isEmpty());
    QVERIFY(info.userId().isEmpty());
    QVERIFY(info.userName().isEmpty());
}

void ModerationReportInfoTest::shouldLoadReportInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ModerationReportInfo>("reportinfo");

    QTest::addRow("empty") << QStringLiteral("empty") << ModerationReportInfo();
    ModerationReportInfo moderationReportInfo;
    moderationReportInfo.setDescription(QStringLiteral("test report 22222"));
    moderationReportInfo.setReportIdentifier("65278f64533fd6f8588e"_ba);
    moderationReportInfo.setTimeStamp(1697091428125);
    moderationReportInfo.setRoomId("xxib9aFXMGN3o"_ba);
    moderationReportInfo.setUserId("H7Q9djXQ4iShzD9T2"_ba);
    moderationReportInfo.setUserName(QStringLiteral("bla"));

    QTest::addRow("moderationinfo1") << QStringLiteral("moderationinfo1") << moderationReportInfo;
}

void ModerationReportInfoTest::shouldLoadReportInfo()
{
    QFETCH(QString, name);
    QFETCH(ModerationReportInfo, reportinfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/moderationreportinfo/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    ModerationReportInfo m;
    m.parseModerationReportInfo(obj);
    QCOMPARE(m, reportinfo);
}

#include "moc_moderationreportinfotest.cpp"
