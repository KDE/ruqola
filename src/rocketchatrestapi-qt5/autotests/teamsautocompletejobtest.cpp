/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsautocompletejobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamsautocompletejob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(TeamsAutoCompleteJobTest)
using namespace RocketChatRestApi;
TeamsAutoCompleteJobTest::TeamsAutoCompleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsAutoCompleteJobTest::shouldHaveDefaultValue()
{
    TeamsAutoCompleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.name().isEmpty());
}

void TeamsAutoCompleteJobTest::shouldGenerateRequest()
{
    TeamsAutoCompleteJob job;
    const QString name = QStringLiteral("blu");
    job.setName(name);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.autocomplete?name=%1").arg(name)));
}

void TeamsAutoCompleteJobTest::shouldNotStarting()
{
    TeamsAutoCompleteJob job;

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
    const QString name = QStringLiteral("foo1");
    job.setName(name);
    QVERIFY(job.canStart());
}

#include "moc_teamsautocompletejobtest.cpp"
