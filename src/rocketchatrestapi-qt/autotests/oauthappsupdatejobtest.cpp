/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsupdatejobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    // QVERIFY(job.permissions().isEmpty());
}

void OauthAppsUpdateJobTest::shouldGenerateRequest()
{
    OauthAppsUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/oauth-apps.update"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void OauthAppsUpdateJobTest::shouldGenerateJson()
{
    OauthAppsUpdateJob job;
    OauthAppsUpdateJob::OauthAppsUpdateInfo foo;
    job.setOauthAppsUpdateInfo(foo);
    foo.name = u"bla"_s;
    foo.redirectUri = u"bl"_s;
    foo.active = true;
    foo.appId = u"bli"_s;
    job.setOauthAppsUpdateInfo(foo);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"active":true,"appId":"bli","name":"bla","redirectUri":"bl"})").toLatin1());
}

void OauthAppsUpdateJobTest::shouldNotStarting()
{
    OauthAppsUpdateJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());

    OauthAppsUpdateJob::OauthAppsUpdateInfo foo;
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(!job.canStart());

    foo.name = u"bla"_s;
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(!job.canStart());

    foo.redirectUri = u"bl"_s;
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(!job.canStart());

    foo.active = true;
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(!job.canStart());

    foo.appId = u"vvs"_s;
    job.setOauthAppsUpdateInfo(foo);
    QVERIFY(job.canStart());
}

#include "moc_oauthappsupdatejobtest.cpp"
