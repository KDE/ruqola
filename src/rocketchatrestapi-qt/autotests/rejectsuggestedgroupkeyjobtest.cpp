/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rejectsuggestedgroupkeyjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "e2e/resetowne2ekeyjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RejectSuggestedGroupKeyJobTest)
using namespace RocketChatRestApi;
RejectSuggestedGroupKeyJobTest::RejectSuggestedGroupKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void RejectSuggestedGroupKeyJobTest::shouldHaveDefaultValue()
{
    ResetOwnE2eKeyJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RejectSuggestedGroupKeyJobTest::shouldGenerateRequest()
{
    ResetOwnE2eKeyJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.resetOwnE2EKey"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RejectSuggestedGroupKeyJobTest::shouldGenerateJson()
{
    ResetOwnE2eKeyJob job;
    // TODO
    // QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"private_key":"%2","public_key":"%1"})").arg(rsapublic, rsaprivate).toLatin1());
}

void RejectSuggestedGroupKeyJobTest::shouldNotStarting()
{
    ResetOwnE2eKeyJob job;

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
    // QVERIFY(!job.canStart());
    QVERIFY(job.canStart());
}

#include "moc_rejectsuggestedgroupkeyjobtest.cpp"
