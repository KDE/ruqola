/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appmarketplacejobtest.h"
#include "apps/appcountjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppMarketPlaceJobTest)
using namespace RocketChatRestApi;
AppMarketPlaceJobTest::AppMarketPlaceJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppMarketPlaceJobTest::shouldHaveDefaultValue()
{
    AppCountJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppMarketPlaceJobTest::shouldGenerateRequest()
{
    AppCountJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/count")));
}

#include "moc_appmarketplacejobtest.cpp"
