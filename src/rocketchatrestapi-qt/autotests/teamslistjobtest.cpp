/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamslistjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "teams/teamslistjob.h"
QTEST_GUILESS_MAIN(TeamsListJobTest)
using namespace RocketChatRestApi;
TeamsListJobTest::TeamsListJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsListJobTest::shouldHaveDefaultValue()
{
    TeamsListJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.pattern().isEmpty());
}

void TeamsListJobTest::shouldGenerateRequest()
{
    TeamsListJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.list"_s));
}

void TeamsListJobTest::shouldGenerateRequestUsername()
{
    TeamsListJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.list"_s));
}

#include "moc_teamslistjobtest.cpp"
