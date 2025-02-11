/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetroomkeyjobtest.h"
#include "e2e/resetowne2ekeyjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ResetRoomKeyJobTest)
using namespace RocketChatRestApi;
ResetRoomKeyJobTest::ResetRoomKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void ResetRoomKeyJobTest::shouldHaveDefaultValue()
{
    ResetOwnE2eKeyJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ResetRoomKeyJobTest::shouldGenerateRequest()
{
    ResetOwnE2eKeyJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.resetOwnE2EKey")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ResetRoomKeyJobTest::shouldGenerateJson()
{
    ResetOwnE2eKeyJob job;
    // TODO
    // QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"private_key":"%2","public_key":"%1"})").arg(rsapublic, rsaprivate).toLatin1());
}

void ResetRoomKeyJobTest::shouldNotStarting()
{
    ResetOwnE2eKeyJob job;

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
    // QVERIFY(!job.canStart());
    QVERIFY(job.canStart());
}

#include "moc_resetroomkeyjobtest.cpp"
