/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resettotpjobtest.h"
#include "restapimethod.h"
#include "users/resettotpjob.h"
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
    ResetTOTPJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
    QVERIFY(job.resetUserId().isEmpty());
}

void ResetTOTPJobTest::shouldGenerateRequest()
{
    ResetTOTPJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetTOTP")));
}

void ResetTOTPJobTest::shouldGenerateJson()
{
    ResetTOTPJob job;
    const QString resetUserId = QStringLiteral("foo");
    job.setResetUserId(resetUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"userId":"%1"})").arg(resetUserId).toLatin1());
}
