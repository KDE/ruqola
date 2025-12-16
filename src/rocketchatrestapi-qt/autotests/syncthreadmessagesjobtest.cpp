/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syncthreadmessagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/syncthreadmessagesjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString threadMessageId = u"bla"_s;
    job.setThreadMessageId(threadMessageId);

    const QString timestamp = u"blu"_s;
    job.setTimeStamp(timestamp);

    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.syncThreadMessages?tmid=%1&updatedSince=%2"_s.arg(threadMessageId, timestamp)));
}

void SyncThreadMessagesJobTest::shouldNotStarting()
{
    SyncThreadMessagesJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString threadMessageId = u"foo1"_s;
    job.setThreadMessageId(threadMessageId);
    QVERIFY(!job.canStart());

    const QString timestamp = u"blu"_s;
    job.setTimeStamp(timestamp);
    QVERIFY(job.canStart());
}

#include "moc_syncthreadmessagesjobtest.cpp"
