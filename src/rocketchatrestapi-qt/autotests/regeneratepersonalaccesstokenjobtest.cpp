/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "regeneratepersonalaccesstokenjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "personalaccesstoken/regeneratepersonalaccesstokenjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
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
    QVERIFY(job.requireTwoFactorAuthentication());
}

void RegeneratePersonalAccessTokenJobTest::shouldGenerateRequest()
{
    RegeneratePersonalAccessTokenJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.regeneratePersonalAccessToken"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RegeneratePersonalAccessTokenJobTest::shouldGenerateJson()
{
    RegeneratePersonalAccessTokenJob job;
    const QString tokenName = u"foo1"_s;
    job.setTokenName(tokenName);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"tokenName":"%1"})").arg(tokenName).toLatin1());
}

void RegeneratePersonalAccessTokenJobTest::shouldNotStarting()
{
    RegeneratePersonalAccessTokenJob job;

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

    job.setAuthCode(u"bla"_s);
    QVERIFY(!job.canStart());

    job.setAuthMethod(u"method"_s);
    QVERIFY(!job.canStart());

    const QString tokenName = u"foo1"_s;
    job.setTokenName(tokenName);
    QVERIFY(job.canStart());
}

#include "moc_regeneratepersonalaccesstokenjobtest.cpp"
