/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setroomkeyidjobtest.h"

#include "e2e/setroomkeyidjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SetRoomKeyIDJobTest)
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
SetRoomKeyIDJobTest::SetRoomKeyIDJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetRoomKeyIDJobTest::shouldHaveDefaultValue()
{
    SetRoomKeyIDJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());

    QVERIFY(!job.setUserPublicAndPrivateKeysInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetRoomKeyIDJobTest::shouldGenerateRequest()
{
    SetRoomKeyIDJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.setUserPublicAndPrivateKeys"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetRoomKeyIDJobTest::shouldGenerateJson()
{
    SetRoomKeyIDJob job;
    SetRoomKeyIDJob::SetUserPublicAndPrivateKeysInfo info;
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

void SetRoomKeyIDJobTest::shouldNotStarting()
{
    SetRoomKeyIDJob job;

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

    SetRoomKeyIDJob::SetUserPublicAndPrivateKeysInfo info;
    const QString rsapublic = u"foo1"_s;
    const QString rsaprivate = u"private"_s;
    info.rsaPrivateKey = rsaprivate;
    job.setSetUserPublicAndPrivateKeysInfo(info);
    QVERIFY(!job.canStart());
    info.rsaPublicKey = rsapublic;
    job.setSetUserPublicAndPrivateKeysInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_setroomkeyidjobtest.cpp"
