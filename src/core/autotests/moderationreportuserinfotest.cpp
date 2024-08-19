/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfotest.h"
#include "moderation/moderationreportuserinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ModerationReportUserInfoTest)
ModerationReportUserInfoTest::ModerationReportUserInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationReportUserInfoTest::shouldHaveDefaultValues()
{
    ModerationReportUserInfo w;
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.reportId().isEmpty());
}

void ModerationReportUserInfoTest::shouldLoadReportUserInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ModerationReportUserInfo>("reportuserinfo");

    {
        ModerationReportUserInfo moderationReportUserInfo;
        User user;
        user.setStatus(User::PresenceStatus::Unknown);
        moderationReportUserInfo.setReportedUser(user);
        moderationReportUserInfo.setReportedBy(user);
        QTest::addRow("empty") << QStringLiteral("empty") << moderationReportUserInfo;
    }
    {
        ModerationReportUserInfo moderationReportUserInfo;
#if 0
    moderationReportInfo.setDescription(QStringLiteral("test report 22222"));
    moderationReportInfo.setReportIdentifier(QByteArrayLiteral("65278f64533fd6f8588e"));
    moderationReportInfo.setTimeStamp(1697091428125);
    moderationReportInfo.setRoomId(QByteArrayLiteral("xxib9aFXMGN3o"));
    moderationReportInfo.setUserId(QByteArrayLiteral("H7Q9djXQ4iShzD9T2"));
    moderationReportInfo.setUserName(QStringLiteral("bla"));

#endif
    QTest::addRow("test1") << QStringLiteral("test1") << moderationReportUserInfo;
    }
}

void ModerationReportUserInfoTest::shouldLoadReportUserInfo()
{
    QFETCH(QString, name);
    QFETCH(ModerationReportUserInfo, reportuserinfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/moderationreportuserinfo/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    ModerationReportUserInfo m;
    m.parseModerationReportUserInfo(obj);
    QCOMPARE(m, reportuserinfo);
}

#include "moc_moderationreportuserinfotest.cpp"
