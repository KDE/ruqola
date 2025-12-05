/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appmarketplacejobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppMarketPlaceJobTest::shouldGenerateRequest()
{
    AppMarketPlaceJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/marketplace?isAdminUser=false"_s));
    }
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        job.setIsAdminUser(true);
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/marketplace?isAdminUser=true"_s));
    }
}

#include "moc_appmarketplacejobtest.cpp"
