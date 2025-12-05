/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssendwelcomeemailjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.email().isEmpty());
}

void UsersSendWelcomeEmailJobTest::shouldGenerateRequest()
{
    UsersSendWelcomeEmailJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.sendWelcomeEmail"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UsersSendWelcomeEmailJobTest::shouldGenerateJson()
{
    UsersSendWelcomeEmailJob job;
    const QString email = u"bla@kde.org"_s;
    job.setEmail(email);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"email":"bla@kde.org"})").toLatin1());
}

void UsersSendWelcomeEmailJobTest::shouldNotStarting()
{
    UsersSendWelcomeEmailJob job;

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
    const QString email{u"foo"_s};
    job.setEmail(email);
    QVERIFY(job.canStart());
}

#include "moc_userssendwelcomeemailjobtest.cpp"
