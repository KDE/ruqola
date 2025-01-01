/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssendwelcomeemailjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/userssendwelcomeemailjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UsersSendWelcomeEmailJobTest)
using namespace RocketChatRestApi;
UsersSendWelcomeEmailJobTest::UsersSendWelcomeEmailJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersSendWelcomeEmailJobTest::shouldHaveDefaultValue()
{
    UsersSendWelcomeEmailJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.email().isEmpty());
}

void UsersSendWelcomeEmailJobTest::shouldGenerateRequest()
{
    UsersSendWelcomeEmailJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.sendWelcomeEmail")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UsersSendWelcomeEmailJobTest::shouldGenerateJson()
{
    UsersSendWelcomeEmailJob job;
    const QString email = QStringLiteral("bla@kde.org");
    job.setEmail(email);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"email":"bla@kde.org"})").toLatin1());
}

void UsersSendWelcomeEmailJobTest::shouldNotStarting()
{
    UsersSendWelcomeEmailJob job;

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
    const QString email{QStringLiteral("foo")};
    job.setEmail(email);
    QVERIFY(job.canStart());
}

#include "moc_userssendwelcomeemailjobtest.cpp"
