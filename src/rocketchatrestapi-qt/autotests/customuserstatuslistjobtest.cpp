/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuslistjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
        const QString authToken = u"foo"_s;
        const QString userId = u"user"_s;
        job.setUserId(userId);
        job.setAuthToken(authToken);
        RestApiMethod method;
        method.setServerUrl(u"http://www.kde.org"_s);
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-user-status.list"_s));
        QCOMPARE(request.rawHeader("X-Auth-Token"_ba), authToken.toLocal8Bit());
        QCOMPARE(request.rawHeader("X-User-Id"_ba), userId.toLocal8Bit());
    }
}

#include "moc_customuserstatuslistjobtest.cpp"
