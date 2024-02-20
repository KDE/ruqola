/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsdeletejobtest.h"
#include "misc/oauthappsdeletejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(OauthAppsDeleteJobTest)
using namespace RocketChatRestApi;
OauthAppsDeleteJobTest::OauthAppsDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void OauthAppsDeleteJobTest::shouldHaveDefaultValue()
{
    OauthAppsDeleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.identifier().isEmpty());
}

void OauthAppsDeleteJobTest::shouldGenerateRequest()
{
    OauthAppsDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void OauthAppsDeleteJobTest::shouldGenerateJson()
{
    OauthAppsDeleteJob job;
    job.setIdentifier(QStringLiteral("bla"));

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"appId":"bla"})").toLatin1());
}

void OauthAppsDeleteJobTest::shouldNotStarting()
{
    OauthAppsDeleteJob job;

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

    job.setIdentifier(QStringLiteral("bla"));
    QVERIFY(job.canStart());
}

#include "moc_oauthappsdeletejobtest.cpp"
