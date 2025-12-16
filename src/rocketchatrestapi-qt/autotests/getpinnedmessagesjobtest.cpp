/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getpinnedmessagesjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/getpinnedmessagesjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QByteArray roomId("bla");
    job.setRoomId(roomId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.getPinnedMessages?roomId=%1"_s.arg(QLatin1StringView(roomId))));
}

void GetPinnedMessagesJobTest::shouldNotStarting()
{
    GetPinnedMessagesJob job;

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

#include "moc_getpinnedmessagesjobtest.cpp"
