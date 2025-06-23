/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetowne2ekeyjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "e2e/resetowne2ekeyjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ResetOwnE2eKeyJobTest)
using namespace RocketChatRestApi;
ResetOwnE2eKeyJobTest::ResetOwnE2eKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void ResetOwnE2eKeyJobTest::shouldHaveDefaultValue()
{
    ResetOwnE2eKeyJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ResetOwnE2eKeyJobTest::shouldGenerateRequest()
{
    ResetOwnE2eKeyJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.resetOwnE2EKey"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ResetOwnE2eKeyJobTest::shouldGenerateJson()
{
    ResetOwnE2eKeyJob job;
    // TODO
    // QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"private_key":"%2","public_key":"%1"})").arg(rsapublic, rsaprivate).toLatin1());
}

void ResetOwnE2eKeyJobTest::shouldNotStarting()
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

#include "moc_resetowne2ekeyjobtest.cpp"
