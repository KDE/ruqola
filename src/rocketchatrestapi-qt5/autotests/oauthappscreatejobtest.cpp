/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappscreatejobtest.h"
#include "misc/oauthappscreatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(OauthAppsCreateJobTest)
using namespace RocketChatRestApi;
OauthAppsCreateJobTest::OauthAppsCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void OauthAppsCreateJobTest::shouldHaveDefaultValue()
{
    OauthAppsCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    // QVERIFY(job.permissions().isEmpty());
}

void OauthAppsCreateJobTest::shouldGenerateRequest()
{
    OauthAppsCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void OauthAppsCreateJobTest::shouldGenerateJson()
{
    OauthAppsCreateJob job;
    OauthAppsCreateJob::OauthAppsCreateInfo foo;
    job.setOauthAppsCreateInfo(foo);
    foo.name = QStringLiteral("bla");
    foo.redirectUri = QStringLiteral("bl");
    foo.active = true;
    job.setOauthAppsCreateInfo(foo);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"active":true,"name":"bla","redirectUri":"bl"})").toLatin1());
}

void OauthAppsCreateJobTest::shouldNotStarting()
{
    OauthAppsCreateJob job;

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

    OauthAppsCreateJob::OauthAppsCreateInfo foo;
    job.setOauthAppsCreateInfo(foo);
    QVERIFY(!job.canStart());

    foo.name = QStringLiteral("bla");
    job.setOauthAppsCreateInfo(foo);
    QVERIFY(!job.canStart());

    foo.redirectUri = QStringLiteral("bl");
    job.setOauthAppsCreateInfo(foo);
    QVERIFY(job.canStart());

    foo.active = true;
    job.setOauthAppsCreateInfo(foo);
    QVERIFY(job.canStart());
}
