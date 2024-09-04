/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

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
        QTest::addRow("empty") << QStringLiteral("empty") << moderationReportUserInfos;
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

        moderationReportUserInfos.setUser(user);
#if 0
        Actual   (m)              :  CreatedAt  QDateTime(2024-01-31 16:08:42.339 UTC Qt::UTC)  Last Login  QDateTime(Invalid)
           description:  "Test report user in ruqola" reportId:  "6638bf943b610b8aa5d35b3f" reportedUser  Name  "kde2"  UserId  "sddf5454"  Status  User::PresenceStatus::Unknown  UserName  "kde2"  UtcOffset  0  StatusText  ""  roles  QList()  CreatedAt  QDateTime(2024-01-31 16:08:42.339 UTC Qt::UTC)  Last Login  QDateTime(Invalid)  userEmailsInfo  email  "foo@kde.org" verified  true  active  true  mRequirePasswordChange  false  mBio  ""  mNickName  "" reportedBy  Name  "bli bli"  UserId  "H7Q9djXQ4iShzD9T2"  Status  User::PresenceStatus::Unknown  UserName  "blu"  UtcOffset  0  StatusText  ""  roles  QList()  CreatedAt  QDateTime(2018-03-13 16:11:51.761 UTC Qt::UTC)  Last Login  QDateTime(Invalid)  userEmailsInfo  email  "" verified  false  active  true  mRequirePasswordChange  false  mBio  ""  mNickName  ""
      description:  "test only !" reportId:  "66361aa93b610b8aa5d35b3c" reportedUser  Name  "kde2"  UserId  "sddf5454"  Status  User::PresenceStatus::Unknown  UserName  "kde2"  UtcOffset  0  StatusText  ""  roles  QList()  CreatedAt  QDateTime(2024-01-31 16:08:42.339 UTC Qt::UTC)  Last Login  QDateTime(Invalid)  userEmailsInfo  email  "foo@kde.org" verified  true  active  true  mRequirePasswordChange  false  mBio  ""  mNickName  "" reportedBy  Name  "bli bli"  UserId  "H7Q9djXQ4iShzD9T2"  Status  User::PresenceStatus::Unknown  UserName  "blu"  UtcOffset  0  StatusText  ""  roles  QList()  CreatedAt  QDateTime(2018-03-13 16:11:51.761 UTC Qt::UTC)  Last Login  QDateTime(Invalid)  userEmailsInfo  email  "" verified  false  active  true  mRequirePasswordChange  false  mBio  ""  mNickName  ""

#endif
        QTest::addRow("test1") << QStringLiteral("test1") << moderationReportUserInfos;
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
