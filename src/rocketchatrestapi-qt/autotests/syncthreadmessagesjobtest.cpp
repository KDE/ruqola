/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syncthreadmessagesjobtest.h"
#include "chat/syncthreadmessagesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(SyncThreadMessagesJobTest)
using namespace RocketChatRestApi;
SyncThreadMessagesJobTest::SyncThreadMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void SyncThreadMessagesJobTest::shouldHaveDefaultValue()
{
    SyncThreadMessagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.threadMessageId().isEmpty());
    QVERIFY(job.timeStamp().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SyncThreadMessagesJobTest::shouldGenerateRequest()
{
    SyncThreadMessagesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString threadMessageId = QStringLiteral("bla");
    job.setThreadMessageId(threadMessageId);

    const QString timestamp = QStringLiteral("blu");
    job.setTimeStamp(timestamp);

    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.syncThreadMessages?tmid=%1&updatedSince=%2").arg(threadMessageId, timestamp)));
}

void SyncThreadMessagesJobTest::shouldNotStarting()
{
    SyncThreadMessagesJob job;

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
    QVERIFY(!job.canStart());

    const QString timestamp = QStringLiteral("blu");
    job.setTimeStamp(timestamp);
    QVERIFY(job.canStart());
}

#include "moc_syncthreadmessagesjobtest.cpp"
