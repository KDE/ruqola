/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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
        User reportedUser;
        reportedUser.setName("test1"_L1);
        reportedUser.setUserName("test1"_L1);
        reportedUser.setStatus(User::PresenceStatus::Unknown);
        reportedUser.setUserId(QByteArrayLiteral("tXCejybNBEvjHcJ6F"));
        User::UserEmailsInfo info;
        info.email = "foo@kde.org"_L1;
        reportedUser.setUserEmailsInfo(info);
        reportedUser.setCreatedAt(QDateTime(QDate(2024, 01, 31), QTime(12, 40, 12, 917), QTimeZone::UTC));
        moderationReportUserInfo.setReportedUser(reportedUser);
        User reportedBy;
        reportedBy.setName("bla"_L1);
        reportedBy.setUserName("doo"_L1);
        reportedBy.setStatus(User::PresenceStatus::Unknown);
        reportedBy.setUserId(QByteArrayLiteral("H7Q9djXQ4iSD9T2"));
        reportedBy.setCreatedAt(QDateTime(QDate(2018, 03, 13), QTime(16, 11, 51, 761), QTimeZone::UTC));

        moderationReportUserInfo.setReportedBy(reportedBy);
        moderationReportUserInfo.setDescription("test signal user (only test :) )"_L1);
        moderationReportUserInfo.setReportId(QByteArrayLiteral("663608573b610aa5d35b3b"));
        moderationReportUserInfo.setTimeStamp(1714817111681);

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
