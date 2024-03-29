/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamslistroomsjobtest.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamslistroomsjob.h"
QTEST_GUILESS_MAIN(TeamsListRoomsJobTest)
using namespace RocketChatRestApi;
TeamsListRoomsJobTest::TeamsListRoomsJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsListRoomsJobTest::shouldHaveDefaultValue()
{
    TeamsListRoomsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void TeamsListRoomsJobTest::shouldGenerateRequest()
{
    TeamsListRoomsJob job;
    const QByteArray teamId("blu");
    job.setTeamId(teamId);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.listRooms?teamId=%1").arg(QString::fromLatin1(teamId))));
}

void TeamsListRoomsJobTest::shouldNotStarting()
{
    TeamsListRoomsJob job;

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
    const QByteArray teamId("foo1");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_teamslistroomsjobtest.cpp"
