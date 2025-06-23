/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appinstalledjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "apps/appcountjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppInstalledJobTest)
using namespace RocketChatRestApi;
AppInstalledJobTest::AppInstalledJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppInstalledJobTest::shouldHaveDefaultValue()
{
    AppCountJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppInstalledJobTest::shouldGenerateRequest()
{
    AppCountJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/count"_s));
}

#include "moc_appinstalledjobtest.cpp"
