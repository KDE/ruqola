/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsautocompletejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include "teams/teamsautocompletejob.h"
#include <QTest>

QTEST_GUILESS_MAIN(TeamsAutoCompleteJobTest)
using namespace RocketChatRestApi;
TeamsAutoCompleteJobTest::TeamsAutoCompleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsAutoCompleteJobTest::shouldHaveDefaultValue()
{
    TeamsAutoCompleteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.name().isEmpty());
}

void TeamsAutoCompleteJobTest::shouldGenerateRequest()
{
    TeamsAutoCompleteJob job;
    const QString name = u"blu"_s;
    job.setName(name);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.autocomplete?name=%1"_s.arg(name)));
}

void TeamsAutoCompleteJobTest::shouldNotStarting()
{
    TeamsAutoCompleteJob job;

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
    const QString name = u"foo1"_s;
    job.setName(name);
    QVERIFY(job.canStart());
}

#include "moc_teamsautocompletejobtest.cpp"
