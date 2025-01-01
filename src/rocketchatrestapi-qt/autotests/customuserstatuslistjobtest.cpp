/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuslistjobtest.h"
#include "custom/customuserstatuslistjob.h"
#include "restapimethod.h"
#include <QTest>
QTEST_GUILESS_MAIN(CustomUserStatusListJobTest)
using namespace RocketChatRestApi;
CustomUserStatusListJobTest::CustomUserStatusListJobTest(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusListJobTest::shouldHaveDefaultValue()
{
    CustomUserStatusListJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void CustomUserStatusListJobTest::shouldGenerateRequest()
{
    {
        CustomUserStatusListJob job;
        const QString authToken = QStringLiteral("foo");
        const QString userId = QStringLiteral("user");
        job.setUserId(userId);
        job.setAuthToken(authToken);
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.list")));
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
    }
}

#include "moc_customuserstatuslistjobtest.cpp"
