/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appfeaturedappsjobtest.h"
#include "apps/appcategoriesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppFeaturedAppsJobTest)
using namespace RocketChatRestApi;
AppFeaturedAppsJobTest::AppFeaturedAppsJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppFeaturedAppsJobTest::shouldHaveDefaultValue()
{
    AppCategoriesJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppFeaturedAppsJobTest::shouldGenerateRequest()
{
    AppCategoriesJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/categories")));
}

#include "moc_appfeaturedappsjobtest.cpp"
