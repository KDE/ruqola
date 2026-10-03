/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssendwelcomeemailjobtest.h"

#include "ruqola_restapi_helper.h"

#include "users/userssendwelcomeemailjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(UsersSendWelcomeEmailJobTest)
using namespace Qt::Literals::StringLiterals;
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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"email":"bla@kde.org"})"_s.toLatin1());
}

void UsersSendWelcomeEmailJobTest::shouldNotStarting()
{
    UsersSendWelcomeEmailJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString email{u"foo"_s};
    job.setEmail(email);
    QVERIFY(job.canStart());
}

#include "moc_userssendwelcomeemailjobtest.cpp"
