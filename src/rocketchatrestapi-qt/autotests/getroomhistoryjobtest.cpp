/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getroomhistoryjobtest.h"

#include "restapimethod.h"
#include "rooms/getroomhistoryjob.h"
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(GetRoomHistoryJobTest)
using namespace RocketChatRestApi;
GetRoomHistoryJobTest::GetRoomHistoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetRoomHistoryJobTest::shouldHaveDefaultValue()
{
    GetRoomHistoryJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.roomHistoryInfo().isValid());

    const GetRoomHistoryJob::GetRoomHistoryInfo info;
    QVERIFY(!info.isValid());
    QVERIFY(info.aroundId.isEmpty());
    QVERIFY(info.lastSeen.isEmpty());
    QVERIFY(info.previous.isEmpty());
    QVERIFY(info.next.isEmpty());
    QVERIFY(info.showThreadMessages);
    QCOMPARE(info.count, 20);
}

void GetRoomHistoryJobTest::shouldGenerateRequest()
{
    GetRoomHistoryJob job;
    const QString roomId{u"foo"_s};
    const GetRoomHistoryJob::GetRoomHistoryInfo info{
        .roomId = "roomId",
        .count = 50,
        .showThreadMessages = true,
    };
    job.setRoomHistoryInfo(info);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.history?roomId=roomId&count=50&showThreadMessages=true"_s));
}

#include "moc_getroomhistoryjobtest.cpp"
