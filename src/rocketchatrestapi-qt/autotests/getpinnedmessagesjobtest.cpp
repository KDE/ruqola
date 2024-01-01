/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getpinnedmessagesjobtest.h"
#include "chat/getpinnedmessagesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(GetPinnedMessagesJobTest)
using namespace RocketChatRestApi;
GetPinnedMessagesJobTest::GetPinnedMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetPinnedMessagesJobTest::shouldHaveDefaultValue()
{
    GetPinnedMessagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void GetPinnedMessagesJobTest::shouldGenerateRequest()
{
    GetPinnedMessagesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("bla");
    job.setRoomId(roomId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getPinnedMessages?roomId=%1").arg(roomId)));
}

void GetPinnedMessagesJobTest::shouldNotStarting()
{
    GetPinnedMessagesJob job;

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

#include "moc_getpinnedmessagesjobtest.cpp"
