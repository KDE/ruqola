/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getthreadmessagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QByteArray threadMessageId("bla");
    job.setThreadMessageId(threadMessageId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.getThreadMessages?tmid=%1"_s.arg(QLatin1StringView(threadMessageId))));
}

void GetThreadMessagesJobTest::shouldNotStarting()
{
    GetThreadMessagesJob job;

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
    const QByteArray threadMessageId("foo1");
    job.setThreadMessageId(threadMessageId);
    QVERIFY(job.canStart());
}

#include "moc_getthreadmessagesjobtest.cpp"
