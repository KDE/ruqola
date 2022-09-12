/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removepersonalaccesstokenjobtest.h"
#include "personalaccesstoken/removepersonalaccesstokenjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(RemovePersonalAccessTokenJobTest)
using namespace RocketChatRestApi;
RemovePersonalAccessTokenJobTest::RemovePersonalAccessTokenJobTest(QObject *parent)
    : QObject(parent)
{
}

void RemovePersonalAccessTokenJobTest::shouldHaveDefaultValue()
{
    RemovePersonalAccessTokenJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.tokenName().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RemovePersonalAccessTokenJobTest::shouldGenerateRequest()
{
    RemovePersonalAccessTokenJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.removePersonalAccessToken")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RemovePersonalAccessTokenJobTest::shouldGenerateJson()
{
    RemovePersonalAccessTokenJob job;
    const QString tokenName = QStringLiteral("foo1");
    job.setTokenName(tokenName);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"tokenName":"%1"})").arg(tokenName).toLatin1());
}

void RemovePersonalAccessTokenJobTest::shouldNotStarting()
{
    RemovePersonalAccessTokenJob job;

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
