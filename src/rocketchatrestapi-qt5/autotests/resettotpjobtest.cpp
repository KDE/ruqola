/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resettotpjobtest.h"
#include "restapimethod.h"
#include "users/forgotpasswordjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ResetTOTPJobTest)
using namespace RocketChatRestApi;
ResetTOTPJobTest::ResetTOTPJobTest(QObject *parent)
    : QObject(parent)
{
}

void ResetTOTPJobTest::shouldHaveDefaultValue()
{
    ForgotPasswordJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.email().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ResetTOTPJobTest::shouldGenerateRequest()
{
    ForgotPasswordJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    job.setEmail(QStringLiteral("foo"));
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.forgotPassword")));
}

void ResetTOTPJobTest::shouldGenerateJson()
{
    ForgotPasswordJob job;
    const QString email = QStringLiteral("foo");
    job.setEmail(email);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"email":"%1"})").arg(email).toLatin1());
}
