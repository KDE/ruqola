/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setstatusjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    QCOMPARE(job.status(), SetStatusJob::StatusType::Unknown);
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SetStatusJobTest::shouldGenerateRequest()
{
    SetStatusJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.setStatus"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetStatusJobTest::shouldGenerateJson()
{
    SetStatusJob job;
    const QString userId = u"foo1"_s;
    job.setStatusUserId(userId);
    job.setStatusMessage(QString());
    job.setStatus(SetStatusJob::StatusType::Away);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"message":"","status":"away","userId":"foo1"})").arg(userId).toLatin1());
    job.setStatusMessage(u"bla"_s);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"message":"bla","status":"away","userId":"foo1"})").arg(userId).toLatin1());
    job.setStatus(SetStatusJob::StatusType::Offline);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"message":"bla","status":"offline","userId":"foo1"})").arg(userId).toLatin1());
}

void SetStatusJobTest::shouldNotStarting()
{
    SetStatusJob job;

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
    const QString statusUserid = u"foo1"_s;
    job.setStatusUserId(statusUserid);
    QVERIFY(!job.canStart());
    job.setStatus(SetStatusJob::StatusType::Away);
    QVERIFY(job.canStart());
}

#include "moc_setstatusjobtest.cpp"
