/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsjobtest.h"
#include "misc/oauthappsjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(OauthAppsJobTest)
using namespace RocketChatRestApi;
OauthAppsJobTest::OauthAppsJobTest(QObject *parent)
    : QObject(parent)
{
}

void OauthAppsJobTest::shouldHaveDefaultValue()
{
    OauthAppsJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void OauthAppsJobTest::shouldGenerateRequest()
{
    OauthAppsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.get")));
}

void OauthAppsJobTest::shouldNotStarting()
{
    OauthAppsJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());

    const QString clientId = QStringLiteral("cli");
    job.setClientId(clientId);
    QVERIFY(!job.canStart());

    const QString appId = QStringLiteral("appId");
    job.setAppId(appId);
    QVERIFY(job.canStart());
}
