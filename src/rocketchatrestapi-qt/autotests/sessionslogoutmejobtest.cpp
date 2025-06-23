/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessionslogoutmejobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/sessions/logout.me"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SessionsLogoutMeJobTest::shouldGenerateJson()
{
    SessionsLogoutMeJob job;
    const QByteArray sessionId("foo1");
    job.setSessionId(sessionId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"sessionId":"%1"})").arg(QLatin1StringView(sessionId)).toLatin1());
}

void SessionsLogoutMeJobTest::shouldNotStarting()
{
    SessionsLogoutMeJob job;

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
    const QByteArray sessionId("foo1");
    job.setSessionId(sessionId);
    QVERIFY(job.canStart());
}

#include "moc_sessionslogoutmejobtest.cpp"
