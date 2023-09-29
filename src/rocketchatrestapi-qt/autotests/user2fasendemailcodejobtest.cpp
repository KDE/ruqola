/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2fasendemailcodejobtest.h"
#include "2fa/user2fasendemailcodejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(User2FASendEmailCodeJobTest)
using namespace RocketChatRestApi;
User2FASendEmailCodeJobTest::User2FASendEmailCodeJobTest(QObject *parent)
    : QObject(parent)
{
}

void User2FASendEmailCodeJobTest::shouldHaveDefaultValue()
{
    User2FASendEmailCodeJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void User2FASendEmailCodeJobTest::shouldGenerateRequest()
{
    User2FASendEmailCodeJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.2fa.sendEmailCode")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void User2FASendEmailCodeJobTest::shouldGenerateJson()
{
    User2FASendEmailCodeJob job;
    const QString emails = QStringLiteral("emails");
    job.setUsernameOrEmail(emails);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emailOrUsername":"%1"})").arg(emails).toLatin1());
}

void User2FASendEmailCodeJobTest::shouldNotStarting()
{
    User2FASendEmailCodeJob job;

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
    job.setUsernameOrEmail(QStringLiteral("emails"));
    QVERIFY(job.canStart());
}

#include "moc_user2fasendemailcodejobtest.cpp"
