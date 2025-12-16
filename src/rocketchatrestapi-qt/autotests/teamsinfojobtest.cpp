/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "teams/teaminfojob.h"
#include <QTest>

QTEST_GUILESS_MAIN(TeamsInfoJobTest)
using namespace RocketChatRestApi;
TeamsInfoJobTest::TeamsInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsInfoJobTest::shouldHaveDefaultValue()
{
    TeamInfoJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.teamId().isEmpty());
}

void TeamsInfoJobTest::shouldGenerateRequest()
{
    TeamInfoJob job;
    const QString teamId = u"blu"_s;
    job.setTeamId(teamId);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.info?teamId=%1"_s.arg(teamId)));
}

void TeamsInfoJobTest::shouldNotStarting()
{
    TeamInfoJob job;

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
    const QString teamId = u"foo1"_s;
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_teamsinfojobtest.cpp"
