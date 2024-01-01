/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsinfojobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teaminfojob.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(TeamsInfoJobTest)
using namespace RocketChatRestApi;
TeamsInfoJobTest::TeamsInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsInfoJobTest::shouldHaveDefaultValue()
{
    TeamInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.teamId().isEmpty());
}

void TeamsInfoJobTest::shouldGenerateRequest()
{
    TeamInfoJob job;
    const QString teamId = QStringLiteral("blu");
    job.setTeamId(teamId);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.info?teamId=%1").arg(teamId)));
}

void TeamsInfoJobTest::shouldNotStarting()
{
    TeamInfoJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString teamId = QStringLiteral("foo1");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_teamsinfojobtest.cpp"
