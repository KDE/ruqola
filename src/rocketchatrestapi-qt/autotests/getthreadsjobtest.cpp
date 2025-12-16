/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getthreadsjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString roomId = u"bla"_s;
    job.setRoomId(roomId);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.getThreadsList?rid=%1&type=all"_s.arg(roomId)));

    job.setSearchType(GetThreadsJob::TheadSearchType::Following);
    request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.getThreadsList?rid=%1&type=following"_s.arg(roomId)));

    job.setSearchType(GetThreadsJob::TheadSearchType::Unread);
    request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.getThreadsList?rid=%1&type=unread"_s.arg(roomId)));

    job.setSearchType(GetThreadsJob::TheadSearchType::All);
    request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.getThreadsList?rid=%1&type=all"_s.arg(roomId)));
}

void GetThreadsJobTest::shouldNotStarting()
{
    GetThreadsJob job;

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
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_getthreadsjobtest.cpp"
