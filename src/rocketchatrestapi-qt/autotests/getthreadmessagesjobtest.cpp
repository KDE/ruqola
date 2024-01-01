/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getthreadmessagesjobtest.h"
#include "chat/getthreadmessagesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(GetThreadMessagesJobTest)
using namespace RocketChatRestApi;
GetThreadMessagesJobTest::GetThreadMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetThreadMessagesJobTest::shouldHaveDefaultValue()
{
    GetThreadMessagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.threadMessageId().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetThreadMessagesJobTest::shouldGenerateRequest()
{
    GetThreadMessagesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString threadMessageId = QStringLiteral("bla");
    job.setThreadMessageId(threadMessageId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getThreadMessages?tmid=%1").arg(threadMessageId)));
}

void GetThreadMessagesJobTest::shouldNotStarting()
{
    GetThreadMessagesJob job;

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
    const QString threadMessageId = QStringLiteral("foo1");
    job.setThreadMessageId(threadMessageId);
    QVERIFY(job.canStart());
}

#include "moc_getthreadmessagesjobtest.cpp"
