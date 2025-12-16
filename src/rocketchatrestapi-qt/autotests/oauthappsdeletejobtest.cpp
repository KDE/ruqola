/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsdeletejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/oauthappsdeletejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(OauthAppsDeleteJobTest)
using namespace RocketChatRestApi;
OauthAppsDeleteJobTest::OauthAppsDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void OauthAppsDeleteJobTest::shouldHaveDefaultValue()
{
    OauthAppsDeleteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.identifier().isEmpty());
}

void OauthAppsDeleteJobTest::shouldGenerateRequest()
{
    OauthAppsDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/oauth-apps.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void OauthAppsDeleteJobTest::shouldGenerateJson()
{
    OauthAppsDeleteJob job;
    job.setIdentifier(u"bla"_s);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"appId":"bla"})").toLatin1());
}

void OauthAppsDeleteJobTest::shouldNotStarting()
{
    OauthAppsDeleteJob job;

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

    job.setIdentifier(u"bla"_s);
    QVERIFY(job.canStart());
}

#include "moc_oauthappsdeletejobtest.cpp"
