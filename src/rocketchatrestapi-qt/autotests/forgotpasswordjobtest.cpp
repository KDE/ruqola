/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "forgotpasswordjobtest.h"
#include "restapimethod.h"
#include "users/forgotpasswordjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ForgotPasswordJobTest)
using namespace RocketChatRestApi;
ForgotPasswordJobTest::ForgotPasswordJobTest(QObject *parent)
    : QObject(parent)
{
}

void ForgotPasswordJobTest::shouldHaveDefaultValue()
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

void ForgotPasswordJobTest::shouldGenerateRequest()
{
    ForgotPasswordJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    job.setEmail(QStringLiteral("foo"));
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.forgotPassword")));
}

void ForgotPasswordJobTest::shouldGenerateJson()
{
    ForgotPasswordJob job;
    const QString email = QStringLiteral("foo");
    job.setEmail(email);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"email":"%1"})").arg(email).toLatin1());
}

#include "moc_forgotpasswordjobtest.cpp"
