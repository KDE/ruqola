/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setuserpublicandprivatekeysjobtest.h"
#include "e2e/setuserpublicandprivatekeysjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SetUserPublicAndPrivateKeysJobTest)
using namespace RocketChatRestApi;
SetUserPublicAndPrivateKeysJobTest::SetUserPublicAndPrivateKeysJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetUserPublicAndPrivateKeysJobTest::shouldHaveDefaultValue()
{
    SetUserPublicAndPrivateKeysJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.rsaPrivateKey().isEmpty());
    QVERIFY(job.rsaPublicKey().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetUserPublicAndPrivateKeysJobTest::shouldGenerateRequest()
{
    SetUserPublicAndPrivateKeysJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.setUserPublicAndPrivateKeys")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SetUserPublicAndPrivateKeysJobTest::shouldGenerateJson()
{
    SetUserPublicAndPrivateKeysJob job;
    const QString rsapublic = QStringLiteral("foo1");
    job.setRsaPublicKey(rsapublic);
    const QString rsaprivate = QStringLiteral("private");
    job.setRsaPrivateKey(rsaprivate);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"private_key":"%2","public_key":"%1"})").arg(rsapublic, rsaprivate).toLatin1());
}

void SetUserPublicAndPrivateKeysJobTest::shouldNotStarting()
{
    SetUserPublicAndPrivateKeysJob job;

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
    const QString rsaprivate = QStringLiteral("foo1");
    job.setRsaPrivateKey(rsaprivate);
    QVERIFY(!job.canStart());

    const QString rsapublic = QStringLiteral("bla");
    job.setRsaPublicKey(rsapublic);
    QVERIFY(job.canStart());
}

#include "moc_setuserpublicandprivatekeysjobtest.cpp"
