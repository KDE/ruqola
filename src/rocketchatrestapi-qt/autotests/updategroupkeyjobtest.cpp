/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updategroupkeyjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "e2e/updategroupkeyjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UpdateGroupKeyJobTest)
using namespace RocketChatRestApi;
UpdateGroupKeyJobTest::UpdateGroupKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void UpdateGroupKeyJobTest::shouldHaveDefaultValue()
{
    UpdateGroupKeyJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.updateGroupInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UpdateGroupKeyJobTest::shouldGenerateRequest()
{
    UpdateGroupKeyJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.updateGroupKey"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UpdateGroupKeyJobTest::shouldGenerateJson()
{
    //    UpdateGroupKeyJob job;
    //    const QString rsapublic = u"foo1"_s;
    //    job.setRsaPublicKey(rsapublic);
    //    const QString rsaprivate = u"private"_s;
    //    job.setRsaPrivateKey(rsaprivate);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"private_key":"%2","public_key":"%1"})").arg(rsapublic,
    //    rsaprivate).toLatin1());
}

void UpdateGroupKeyJobTest::shouldNotStarting()
{
    UpdateGroupKeyJob job;

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
    // TODO add update info
    // FIXME QVERIFY(job.canStart());
}

#include "moc_updategroupkeyjobtest.cpp"
