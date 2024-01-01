/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statisticsjobtest.h"
#include "misc/statisticsjob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(StatisticsJobTest)
using namespace RocketChatRestApi;
StatisticsJobTest::StatisticsJobTest(QObject *parent)
    : QObject(parent)
{
}

void StatisticsJobTest::shouldHaveDefaultValue()
{
    StatisticsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.refresh());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void StatisticsJobTest::shouldGenerateRequest()
{
    StatisticsJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/statistics?refresh=false")));
    }
    {
        job.setRefresh(true);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/statistics?refresh=true")));
    }
}

#include "moc_statisticsjobtest.cpp"
