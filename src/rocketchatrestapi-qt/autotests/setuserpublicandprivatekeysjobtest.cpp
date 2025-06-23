/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setuserpublicandprivatekeysjobtest.h"
using namespace Qt::Literals::StringLiterals;

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

    QVERIFY(!job.setUserPublicAndPrivateKeysInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetUserPublicAndPrivateKeysJobTest::shouldGenerateRequest()
{
    SetUserPublicAndPrivateKeysJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.setUserPublicAndPrivateKeys"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetUserPublicAndPrivateKeysJobTest::shouldGenerateJson()
{
    SetUserPublicAndPrivateKeysJob job;
    SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysInfo info;
    const QString rsapublic = u"foo1"_s;
    const QString rsaprivate = u"private"_s;
    info.rsaPrivateKey = rsaprivate;
    info.rsaPublicKey = rsapublic;
    job.setSetUserPublicAndPrivateKeysInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"force":false,"private_key":"%2","public_key":"%1"})").arg(rsapublic, rsaprivate).toLatin1());

    info.force = true;
    job.setSetUserPublicAndPrivateKeysInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"force":true,"private_key":"%2","public_key":"%1"})").arg(rsapublic, rsaprivate).toLatin1());
}

void SetUserPublicAndPrivateKeysJobTest::shouldNotStarting()
{
    SetUserPublicAndPrivateKeysJob job;

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

    SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysInfo info;
    const QString rsapublic = u"foo1"_s;
    const QString rsaprivate = u"private"_s;
    info.rsaPrivateKey = rsaprivate;
    job.setSetUserPublicAndPrivateKeysInfo(info);
    QVERIFY(!job.canStart());
    info.rsaPublicKey = rsapublic;
    job.setSetUserPublicAndPrivateKeysInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_setuserpublicandprivatekeysjobtest.cpp"
