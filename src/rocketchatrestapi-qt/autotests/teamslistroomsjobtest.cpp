/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamslistroomsjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.listRooms?teamId=%1"_s.arg(QLatin1StringView(teamId))));
}

void TeamsListRoomsJobTest::shouldNotStarting()
{
    TeamsListRoomsJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QByteArray teamId("foo1");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_teamslistroomsjobtest.cpp"
