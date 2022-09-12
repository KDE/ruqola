/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "regeneratepersonalaccesstokenjobtest.h"
#include "personalaccesstoken/regeneratepersonalaccesstokenjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(RegeneratePersonalAccessTokenJobTest)
using namespace RocketChatRestApi;
RegeneratePersonalAccessTokenJobTest::RegeneratePersonalAccessTokenJobTest(QObject *parent)
    : QObject(parent)
{
}

void RegeneratePersonalAccessTokenJobTest::shouldHaveDefaultValue()
{
    RegeneratePersonalAccessTokenJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.tokenName().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RegeneratePersonalAccessTokenJobTest::shouldGenerateRequest()
{
    RegeneratePersonalAccessTokenJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.regeneratePersonalAccessToken")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RegeneratePersonalAccessTokenJobTest::shouldGenerateJson()
{
    RegeneratePersonalAccessTokenJob job;
    const QString tokenName = QStringLiteral("foo1");
    job.setTokenName(tokenName);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"tokenName":"%1"})").arg(tokenName).toLatin1());
}

void RegeneratePersonalAccessTokenJobTest::shouldNotStarting()
{
    RegeneratePersonalAccessTokenJob job;

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
    const QString tokenName = QStringLiteral("foo1");
    job.setTokenName(tokenName);
    QVERIFY(job.canStart());
}
