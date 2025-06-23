/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logoutjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "authentication/logoutjob.h"
#include "restapimethod.h"
#include <QTest>
QTEST_GUILESS_MAIN(LogoutJobTest)
using namespace RocketChatRestApi;
LogoutJobTest::LogoutJobTest(QObject *parent)
    : QObject(parent)
{
}

void LogoutJobTest::shouldHaveDefaultValue()
{
    LogoutJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void LogoutJobTest::shouldGenerateRequest()
{
    LogoutJob job;
    const QString authToken = u"foo"_s;
    const QString userId = u"user"_s;
    job.setUserId(userId);
    job.setAuthToken(authToken);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/logout"_s));
    QCOMPARE(request.rawHeader("X-Auth-Token"_ba), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader("X-User-Id"_ba), userId.toLocal8Bit());
}

#include "moc_logoutjobtest.cpp"
