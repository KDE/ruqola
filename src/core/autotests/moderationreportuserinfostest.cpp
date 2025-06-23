/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfostest.h"
#include "moderation/moderationreportuserinfos.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ModerationReportUserInfosTest)
ModerationReportUserInfosTest::ModerationReportUserInfosTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationReportUserInfosTest::shouldHaveDefaultValues()
{
    ModerationReportUserInfos infos;
    QCOMPARE(infos.moderationReportUserInfosCount(), 0);
    QCOMPARE(infos.offset(), 0);
    QCOMPARE(infos.total(), 0);
    QVERIFY(infos.moderationReportUserInfosList().isEmpty());
    QVERIFY(!infos.user().isValid());
}

void ModerationReportUserInfosTest::shouldLoadReportUserInfos_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ModerationReportUserInfos>("reportuserinfos");

    {
        ModerationReportUserInfos moderationReportUserInfos;
        User user;
        user.setStatus(User::PresenceStatus::Unknown);
        moderationReportUserInfos.setUser(user);
        QTest::addRow("empty") << u"empty"_s << moderationReportUserInfos;
    }

    {
        ModerationReportUserInfos moderationReportUserInfos;
        moderationReportUserInfos.setTotal(2);
        moderationReportUserInfos.setModerationReportUserInfosCount(2);
        User user;
        user.setStatus(User::PresenceStatus::Unknown);
        user.setName("kde2"_L1);
        user.setUserId("sddf5454");
        user.setUserName("kde2"_L1);
        user.setRoles({"user"_L1}, {});
        User::UserEmailsInfo userEmailInfo;
        userEmailInfo.email = "foo@kde.org"_L1;
        userEmailInfo.verified = true;
        user.setUserEmailsInfo(userEmailInfo);
        user.setRequirePasswordChange(false);
        QDateTime createdTime;
        createdTime.setDate(QDate(2024, 01, 31));
        createdTime.setTime(QTime(16, 8, 42, 339));
        createdTime.setTimeZone(QTimeZone::UTC);
        user.setCreatedAt(createdTime);
        moderationReportUserInfos.setUser(user);

        QList<ModerationReportUserInfo> lstUserInfo;
        {
            ModerationReportUserInfo info;
            info.setDescription("Test report user in ruqola"_L1);
            info.setReportId("6638bf943b610b8aa5d35b3f");
            User reportedUser;
            reportedUser.setUserId("sddf5454");
            reportedUser.setName("kde2"_L1);
            reportedUser.setUserName("kde2"_L1);
            reportedUser.setStatus(User::PresenceStatus::Unknown);

            User::UserEmailsInfo userEmailInfo;
            userEmailInfo.email = "foo@kde.org"_L1;
            userEmailInfo.verified = true;
            reportedUser.setUserEmailsInfo(userEmailInfo);

            QDateTime createdTime;
            createdTime.setDate(QDate(2024, 01, 31));
            createdTime.setTime(QTime(16, 8, 42, 339));
            createdTime.setTimeZone(QTimeZone::UTC);
            reportedUser.setCreatedAt(createdTime);
            info.setReportedUser(reportedUser);

            User reportedBy;
            reportedBy.setName("bli bli"_L1);
            reportedBy.setUserName("blu"_L1);
            reportedBy.setStatus(User::PresenceStatus::Unknown);
            reportedBy.setUserId("H7Q9djXQ4iShzD9T2");

            QDateTime createdTimeBy;
            createdTimeBy.setDate(QDate(2018, 03, 13));
            createdTimeBy.setTime(QTime(16, 11, 51, 761));
            createdTimeBy.setTimeZone(QTimeZone::UTC);
            reportedBy.setCreatedAt(createdTimeBy);
            info.setReportedBy(reportedBy);
            info.setTimeStamp(1714995092465);

            lstUserInfo.append(std::move(info));
        }
        {
            ModerationReportUserInfo info;
            info.setDescription("test only !"_L1);
            info.setReportId("66361aa93b610b8aa5d35b3c");
            User reportedUser;
            QDateTime createdTime;
            createdTime.setDate(QDate(2024, 01, 31));
            createdTime.setTime(QTime(16, 8, 42, 339));
            createdTime.setTimeZone(QTimeZone::UTC);
            reportedUser.setCreatedAt(createdTime);
            reportedUser.setUserId("sddf5454");
            reportedUser.setName("kde2"_L1);
            reportedUser.setUserName("kde2"_L1);
            reportedUser.setStatus(User::PresenceStatus::Unknown);
            User::UserEmailsInfo userEmailInfo;
            userEmailInfo.email = "foo@kde.org"_L1;
            userEmailInfo.verified = true;
            reportedUser.setUserEmailsInfo(userEmailInfo);

            info.setReportedUser(reportedUser);

            User reportedBy;
            reportedBy.setName("bli bli"_L1);
            reportedBy.setUserName("blu"_L1);
            reportedBy.setStatus(User::PresenceStatus::Unknown);
            reportedBy.setUserId("H7Q9djXQ4iShzD9T2");

            QDateTime createdTimeBy;
            createdTimeBy.setDate(QDate(2018, 03, 13));
            createdTimeBy.setTime(QTime(16, 11, 51, 761));
            createdTimeBy.setTimeZone(QTimeZone::UTC);
            reportedBy.setCreatedAt(createdTimeBy);
            info.setReportedBy(reportedBy);
            info.setTimeStamp(1714821801128);
            lstUserInfo.append(std::move(info));
        }

        moderationReportUserInfos.setModerationReportUserInfosList(std::move(lstUserInfo));
#if 0
      reportedUser    userEmailsInfo  email  "" verified  false  active  true  mRequirePasswordChange  false  mBio  ""  mNickName  ""
      reportedUser   userEmailsInfo  email  "" verified  false  active  true  mRequirePasswordChange  false  mBio  ""  mNickName  ""

#endif
        QTest::addRow("test1") << u"test1"_s << moderationReportUserInfos;
    }
}

void ModerationReportUserInfosTest::shouldLoadReportUserInfos()
{
    QFETCH(QString, name);
    QFETCH(ModerationReportUserInfos, reportuserinfos);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/moderationreportuserinfos/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    ModerationReportUserInfos m;
    m.parseModerationReportUserInfos(obj);
    QCOMPARE(m, reportuserinfos);
}

#include "moc_moderationreportuserinfostest.cpp"
