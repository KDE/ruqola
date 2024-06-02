/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appcountjobtest.h"
#include "apps/appcountjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppCountJobTest)
using namespace RocketChatRestApi;
AppCountJobTest::AppCountJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppCountJobTest::shouldHaveDefaultValue()
{
    AppCountJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppCountJobTest::shouldGenerateRequest()
{
    AppCountJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/count")));
}

#include "moc_appcountjobtest.cpp"
