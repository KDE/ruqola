/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmentionedmessagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QByteArray roomId("bla");
    job.setRoomId(roomId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.getMentionedMessages?roomId=%1"_s.arg(QLatin1StringView(roomId))));
}

void GetMentionedMessagesJobTest::shouldNotStarting()
{
    GetMentionedMessagesJob job;

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
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_getmentionedmessagesjobtest.cpp"
