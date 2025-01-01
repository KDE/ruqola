/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appmarketplacejobtest.h"
#include "apps/appmarketplacejob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppMarketPlaceJobTest)
using namespace RocketChatRestApi;
AppMarketPlaceJobTest::AppMarketPlaceJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppMarketPlaceJobTest::shouldHaveDefaultValue()
{
    AppMarketPlaceJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppMarketPlaceJobTest::shouldGenerateRequest()
{
    AppMarketPlaceJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/marketplace?isAdminUser=false")));
    }
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        job.setIsAdminUser(true);
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/marketplace?isAdminUser=true")));
    }
}

#include "moc_appmarketplacejobtest.cpp"
