/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resete2ekeyjobtest.h"
#include "restapimethod.h"
#include "users/resete2ekeyjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ResetE2EKeyJobTest)
using namespace RocketChatRestApi;
ResetE2EKeyJobTest::ResetE2EKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void ResetE2EKeyJobTest::shouldHaveDefaultValue()
{
    ResetE2EKeyJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
}

void ResetE2EKeyJobTest::shouldGenerateRequest()
{
    ResetE2EKeyJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetE2EKey")));
}

void ResetE2EKeyJobTest::shouldGenerateJson()
{
    ResetE2EKeyJob job;
    //    const QString email = QStringLiteral("foo");
    //    job.setEmail(email);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"email":"%1"})").arg(email).toLatin1());
}
