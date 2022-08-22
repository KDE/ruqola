/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updategroupkeyjobtest.h"
#include "e2e/updategroupkeyjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(UpdateGroupKeyJobTest)
using namespace RocketChatRestApi;
UpdateGroupKeyJobTest::UpdateGroupKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void UpdateGroupKeyJobTest::shouldHaveDefaultValue()
{
    UpdateGroupKeyJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.updateGroupInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UpdateGroupKeyJobTest::shouldGenerateRequest()
{
    UpdateGroupKeyJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.updateGroupKey")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UpdateGroupKeyJobTest::shouldGenerateJson()
{
    //    UpdateGroupKeyJob job;
    //    const QString rsapublic = QStringLiteral("foo1");
    //    job.setRsaPublicKey(rsapublic);
    //    const QString rsaprivate = QStringLiteral("private");
    //    job.setRsaPrivateKey(rsaprivate);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"private_key":"%2","public_key":"%1"})").arg(rsapublic,
    //    rsaprivate).toLatin1());
}

void UpdateGroupKeyJobTest::shouldNotStarting()
{
    UpdateGroupKeyJob job;

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
    // TODO add update info
    // FIXME QVERIFY(job.canStart());
}
