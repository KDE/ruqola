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
        moderationReportUserInfos.setUser(user);
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
