/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "spotlightjobtest.h"

#include "misc/spotlightjob.h"
#include "restapimethod.h"
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(SpotlightJobTest)
using namespace RocketChatRestApi;
SpotlightJobTest::SpotlightJobTest(QObject *parent)
    : QObject(parent)
{
}

void SpotlightJobTest::shouldHaveDefaultValue()
{
    SpotlightJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.spotlightInfo().isValid());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());

    const SpotlightJob::SpotlightJobInfo info;
    QVERIFY(info.excludeUsernames.isEmpty());
    QVERIFY(info.pattern.isEmpty());
    QVERIFY(info.roomId.isEmpty());
    QCOMPARE(info.type, SpotlightJob::SpotlightJobType::Unknown);
}

void SpotlightJobTest::shouldGenerateRequest()
{
    SpotlightJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString avatarUserId = u"avat"_s;
    SpotlightJob::SpotlightJobInfo info;
    info.pattern = avatarUserId;
    {
        job.setSpotlightInfo(info);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/spotlight?query=avat"_s));
    }
    {
        info.roomId = "roo32"_ba;
        job.setSpotlightInfo(info);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/spotlight?query=avat&rid=roo32"_s));
    }
    {
        info.roomId = "roo37"_ba;
        info.type = SpotlightJob::SpotlightJobType::Rooms;
        job.setSpotlightInfo(info);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url().toEncoded(), "http://www.kde.org/api/v1/spotlight?query=avat&rid=roo37&type=%7B%22rooms%22:true%7D"_ba);
    }
    {
        info.roomId = "roo39"_ba;
        info.type = SpotlightJob::SpotlightJobType::Rooms;
        info.excludeUsernames = {u"user1"_s, u"user3"_s};
        job.setSpotlightInfo(info);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url().toEncoded(), "http://www.kde.org/api/v1/spotlight?query=avat&rid=roo39&type=%7B%22rooms%22:true%7D&usernames=user1,user3"_ba);
    }
}

#include "moc_spotlightjobtest.cpp"
