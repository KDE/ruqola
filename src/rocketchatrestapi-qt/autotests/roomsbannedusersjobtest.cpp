/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsbannedusersjobtest.h"

#include "restapimethod.h"
#include "rooms/roomsbannedusersjob.h"
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RoomsBannedUsersJobTest)
using namespace RocketChatRestApi;
RoomsBannedUsersJobTest::RoomsBannedUsersJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsBannedUsersJobTest::shouldHaveDefaultValue()
{
    RoomsBannedUsersJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.roomsBannedUsersJobInfo().isValid());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoomsBannedUsersJobTest::shouldGenerateRequest()
{
    {
        RoomsBannedUsersJob job;
        RestApiMethod method;
        method.setServerUrl(u"http://www.kde.org"_s);
        job.setRestApiMethod(&method);
        RoomsBannedUsersJob::RoomsBannedUsersJobInfo info;
        info.roomId = "room_id"_ba;
        info.count = 5;
        info.offset = 0;
        job.setRoomsBannedUsersJobInfo(info);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.bannedUsers?roomId=room_id&offset=0&count=5"_s));
    }
}

void RoomsBannedUsersJobTest::shouldNotStarting()
{
    RoomsBannedUsersJob job;

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
    RoomsBannedUsersJob::RoomsBannedUsersJobInfo info;
    info.roomId = "room_id"_ba;
    info.count = 0;
    info.offset = 0;
    job.setRoomsBannedUsersJobInfo(info);

    QVERIFY(!job.canStart());
    info.count = 4;
    job.setRoomsBannedUsersJobInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_roomsbannedusersjobtest.cpp"
