/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsupdatejobtest.h"
#include "misc/oauthappsupdatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(OauthAppsUpdateJobTest)
using namespace RocketChatRestApi;
OauthAppsUpdateJobTest::OauthAppsUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void OauthAppsUpdateJobTest::shouldHaveDefaultValue()
{
    OauthAppsUpdateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    // QVERIFY(job.permissions().isEmpty());
}

void OauthAppsUpdateJobTest::shouldGenerateRequest()
{
    OauthAppsUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void OauthAppsUpdateJobTest::shouldGenerateJson()
{
    OauthAppsUpdateJob job;
    OauthAppsUpdateJob::OauthAppsUpdateInfo foo;
    job.setOauthAppsUpdateInfo(foo);
    foo.name = QStringLiteral("bla");
    foo.redirectUri = QStringLiteral("bl");
    foo.active = true;
    foo.appId = QStringLiteral("bli");
    job.setOauthAppsUpdateInfo(foo);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"active":true,"appId":"bli","name":"bla","redirectUri":"bl"})").toLatin1());
}

void OauthAppsUpdateJobTest::shouldNotStarting()
{
    OauthAppsUpdateJob job;

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

    OauthAppsUpdateJob::OauthAppsUpdateInfo foo;
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(!job.canStart());

    foo.name = QStringLiteral("bla");
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(!job.canStart());

    foo.redirectUri = QStringLiteral("bl");
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(!job.canStart());

    foo.active = true;
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(!job.canStart());

    foo.appId = QStringLiteral("vvs");
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(job.canStart());
}

#include "moc_oauthappsupdatejobtest.cpp"
