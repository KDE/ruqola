/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamslistjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamslistjob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(TeamsListJobTest)
using namespace RocketChatRestApi;
TeamsListJobTest::TeamsListJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsListJobTest::shouldHaveDefaultValue()
{
    TeamsListJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.pattern().isEmpty());
}

void TeamsListJobTest::shouldGenerateRequest()
{
    TeamsListJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.list")));
}

void TeamsListJobTest::shouldGenerateRequestUsername()
{
    TeamsListJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.list")));
}

#include "moc_teamslistjobtest.cpp"
