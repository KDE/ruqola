/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appfeaturedappsjobtest.h"
#include "apps/appfeaturedappsjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppFeaturedAppsJobTest)
using namespace RocketChatRestApi;
AppFeaturedAppsJobTest::AppFeaturedAppsJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppFeaturedAppsJobTest::shouldHaveDefaultValue()
{
    AppFeaturedAppsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppFeaturedAppsJobTest::shouldGenerateRequest()
{
    AppFeaturedAppsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/featured-apps")));
}

#include "moc_appfeaturedappsjobtest.cpp"
