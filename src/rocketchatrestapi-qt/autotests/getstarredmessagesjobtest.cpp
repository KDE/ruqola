/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getstarredmessagesjobtest.h"
#include "chat/getstarredmessagesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(GetStarredMessagesJobTest)
using namespace RocketChatRestApi;
GetStarredMessagesJobTest::GetStarredMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetStarredMessagesJobTest::shouldHaveDefaultValue()
{
    GetStarredMessagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void GetStarredMessagesJobTest::shouldGenerateRequest()
{
    GetStarredMessagesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QByteArray roomId = QByteArrayLiteral("bla");
    job.setRoomId(roomId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getStarredMessages?roomId=%1").arg(QLatin1StringView(roomId))));
}

void GetStarredMessagesJobTest::shouldNotStarting()
{
    GetStarredMessagesJob job;

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
    const QByteArray roomId = QByteArrayLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_getstarredmessagesjobtest.cpp"
