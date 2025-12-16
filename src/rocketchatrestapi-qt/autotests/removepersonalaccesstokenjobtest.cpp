/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removepersonalaccesstokenjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "personalaccesstoken/removepersonalaccesstokenjob.h"
#include "restapimethod.h"
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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.tokenName().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
}

void RemovePersonalAccessTokenJobTest::shouldGenerateRequest()
{
    RemovePersonalAccessTokenJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.removePersonalAccessToken"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RemovePersonalAccessTokenJobTest::shouldGenerateJson()
{
    RemovePersonalAccessTokenJob job;
    const QString tokenName = u"foo1"_s;
    job.setTokenName(tokenName);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"tokenName":"%1"})").arg(tokenName).toLatin1());
}

void RemovePersonalAccessTokenJobTest::shouldNotStarting()
{
    RemovePersonalAccessTokenJob job;

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

    job.setAuthCode(u"bla"_s);
    QVERIFY(!job.canStart());

    job.setAuthMethod(u"method"_s);
    QVERIFY(!job.canStart());

    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString tokenName = u"foo1"_s;
    job.setTokenName(tokenName);
    QVERIFY(job.canStart());
}

#include "moc_removepersonalaccesstokenjobtest.cpp"
