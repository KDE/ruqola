/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfojobtest.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "serverinfojob.h"
QTEST_GUILESS_MAIN(ServerInfoJobTest)
using namespace RocketChatRestApi;
ServerInfoJobTest::ServerInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void ServerInfoJobTest::shouldHaveDefaultValue()
{
    ServerInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.useDeprecatedVersion());
}

void ServerInfoJobTest::shouldGenerateRequest()
{
    {
        ServerInfoJob job;
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/info")));
        QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
        QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    }
    {
        ServerInfoJob job;
        job.setUseDeprecatedVersion(false);
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/info")));
        QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
        QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    }
}

#include "moc_serverinfojobtest.cpp"
