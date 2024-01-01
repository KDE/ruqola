/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getthreadsjobtest.h"
#include "chat/getthreadsjob.h"
#include "restapimethod.h"
#include <QTest>
QTEST_GUILESS_MAIN(GetThreadsJobTest)
using namespace RocketChatRestApi;
GetThreadsJobTest::GetThreadsJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetThreadsJobTest::shouldHaveDefaultValue()
{
    GetThreadsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QCOMPARE(job.searchType(), GetThreadsJob::TheadSearchType::All);
}

void GetThreadsJobTest::shouldGenerateRequest()
{
    GetThreadsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("bla");
    job.setRoomId(roomId);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getThreadsList?rid=%1&type=all").arg(roomId)));

    job.setSearchType(GetThreadsJob::TheadSearchType::Following);
    request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getThreadsList?rid=%1&type=following").arg(roomId)));

    job.setSearchType(GetThreadsJob::TheadSearchType::Unread);
    request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getThreadsList?rid=%1&type=unread").arg(roomId)));

    job.setSearchType(GetThreadsJob::TheadSearchType::All);
    request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getThreadsList?rid=%1&type=all").arg(roomId)));
}

void GetThreadsJobTest::shouldNotStarting()
{
    GetThreadsJob job;

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

#include "moc_getthreadsjobtest.cpp"
