/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setstatusjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/setstatusjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SetStatusJobTest)
using namespace RocketChatRestApi;
SetStatusJobTest::SetStatusJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetStatusJobTest::shouldHaveDefaultValue()
{
    SetStatusJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.statusUserId().isEmpty());
    QVERIFY(job.statusMessage().isEmpty());
    QCOMPARE(job.status(), SetStatusJob::Unknown);
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SetStatusJobTest::shouldGenerateRequest()
{
    SetStatusJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setStatus")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SetStatusJobTest::shouldGenerateJson()
{
    SetStatusJob job;
    const QString userId = QStringLiteral("foo1");
    job.setStatusUserId(userId);
    job.setStatusMessage(QString());
    job.setStatus(SetStatusJob::Away);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"message":"","status":"away","userId":"foo1"})").arg(userId).toLatin1());
    job.setStatusMessage(QStringLiteral("bla"));
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"message":"bla","status":"away","userId":"foo1"})").arg(userId).toLatin1());
    job.setStatus(SetStatusJob::Offline);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"message":"bla","status":"offline","userId":"foo1"})").arg(userId).toLatin1());
}

void SetStatusJobTest::shouldNotStarting()
{
    SetStatusJob job;

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
    const QString statusUserid = QStringLiteral("foo1");
    job.setStatusUserId(statusUserid);
    QVERIFY(!job.canStart());
    job.setStatus(SetStatusJob::Away);
    QVERIFY(job.canStart());
}

#include "moc_setstatusjobtest.cpp"
