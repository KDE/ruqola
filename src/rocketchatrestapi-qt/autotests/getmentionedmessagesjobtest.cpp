/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmentionedmessagesjobtest.h"
#include "chat/getmentionedmessagesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(GetMentionedMessagesJobTest)
using namespace RocketChatRestApi;
GetMentionedMessagesJobTest::GetMentionedMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetMentionedMessagesJobTest::shouldHaveDefaultValue()
{
    GetMentionedMessagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void GetMentionedMessagesJobTest::shouldGenerateRequest()
{
    GetMentionedMessagesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("bla");
    job.setRoomId(roomId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getMentionedMessages?roomId=%1").arg(roomId)));
}

void GetMentionedMessagesJobTest::shouldNotStarting()
{
    GetMentionedMessagesJob job;

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
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_getmentionedmessagesjobtest.cpp"
