/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessionslogoutmejobtest.h"
#include "ruqola_restapi_helper.h"
#include "sessions/sessionslogoutmejob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SessionsLogoutMeJobTest)
using namespace RocketChatRestApi;
SessionsLogoutMeJobTest::SessionsLogoutMeJobTest(QObject *parent)
    : QObject(parent)
{
}

void SessionsLogoutMeJobTest::shouldHaveDefaultValue()
{
    SessionsLogoutMeJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.sessionId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SessionsLogoutMeJobTest::shouldGenerateRequest()
{
    SessionsLogoutMeJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/sessions/logout.me")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SessionsLogoutMeJobTest::shouldGenerateJson()
{
    SessionsLogoutMeJob job;
    const QString sessionId = QStringLiteral("foo1");
    job.setSessionId(sessionId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"sessionId":"%1"})").arg(sessionId).toLatin1());
}

void SessionsLogoutMeJobTest::shouldNotStarting()
{
    SessionsLogoutMeJob job;

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
    const QString sessionId = QStringLiteral("foo1");
    job.setSessionId(sessionId);
    QVERIFY(job.canStart());
}

#include "moc_sessionslogoutmejobtest.cpp"
