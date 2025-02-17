/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getsnippetedmessagesjobtest.h"
#include "chat/getsnippetedmessagesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(GetSnippetedMessagesJobTest)
using namespace RocketChatRestApi;
GetSnippetedMessagesJobTest::GetSnippetedMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetSnippetedMessagesJobTest::shouldHaveDefaultValue()
{
    GetSnippetedMessagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void GetSnippetedMessagesJobTest::shouldGenerateRequest()
{
    GetSnippetedMessagesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QByteArray roomId = "bla"_ba;
    job.setRoomId(roomId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getSnippetedMessages?roomId=%1").arg(QLatin1StringView(roomId))));
}

void GetSnippetedMessagesJobTest::shouldNotStarting()
{
    GetSnippetedMessagesJob job;

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
    const QByteArray roomId = "foo1"_ba;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_getsnippetedmessagesjobtest.cpp"
