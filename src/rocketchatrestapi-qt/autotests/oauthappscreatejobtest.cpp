/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappscreatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/oauthappscreatejob.h"
#include "restapimethod.h"
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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    // QVERIFY(job.permissions().isEmpty());
}

void OauthAppsCreateJobTest::shouldGenerateRequest()
{
    OauthAppsCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/oauth-apps.create"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void OauthAppsCreateJobTest::shouldGenerateJson()
{
    OauthAppsCreateJob job;
    OauthAppsCreateJob::OauthAppsCreateInfo foo;
    job.setOauthAppsCreateInfo(foo);
    foo.name = u"bla"_s;
    foo.redirectUri = u"bl"_s;
    foo.active = true;
    job.setOauthAppsCreateInfo(foo);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"active":true,"name":"bla","redirectUri":"bl"})").toLatin1());
}

void OauthAppsCreateJobTest::shouldNotStarting()
{
    OauthAppsCreateJob job;

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

    OauthAppsCreateJob::OauthAppsCreateInfo foo;
    job.setOauthAppsCreateInfo(foo);
    QVERIFY(!job.canStart());

    foo.name = u"bla"_s;
    job.setOauthAppsCreateInfo(foo);
    QVERIFY(!job.canStart());

    foo.redirectUri = u"bl"_s;
    job.setOauthAppsCreateInfo(foo);
    QVERIFY(job.canStart());

    foo.active = true;
    job.setOauthAppsCreateInfo(foo);
    QVERIFY(job.canStart());
}

#include "moc_oauthappscreatejobtest.cpp"
