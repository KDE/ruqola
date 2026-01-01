/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statisticsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/statisticsjob.h"

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QTest>

QTEST_GUILESS_MAIN(StatisticsJobTest)
using namespace RocketChatRestApi;
StatisticsJobTest::StatisticsJobTest(QObject *parent)
    : QObject(parent)
{
}

void StatisticsJobTest::shouldHaveDefaultValue()
{
    StatisticsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/statistics?refresh=false"_s));
    }
    {
        job.setRefresh(true);
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/statistics?refresh=true"_s));
    }
}

#include "moc_statisticsjobtest.cpp"
