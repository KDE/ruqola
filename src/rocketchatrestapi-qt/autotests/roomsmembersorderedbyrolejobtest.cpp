/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmembersorderedbyrolejobtest.h"
#include "restapimethod.h"
#include "rooms/roomsmembersorderedbyrolejob.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomsMembersOrderedByRoleJobTest)
using namespace RocketChatRestApi;
RoomsMembersOrderedByRoleJobTest::RoomsMembersOrderedByRoleJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsMembersOrderedByRoleJobTest::shouldHaveDefaultValue()
{
    RoomsMembersOrderedByRoleJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.roomsMembersOrderedByRoleJobInfo().isValid());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoomsMembersOrderedByRoleJobTest::shouldGenerateRequest()
{
    RoomsMembersOrderedByRoleJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QByteArray roomId("bla");
    RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo info;
    info.roomId = "room_id"_ba;
    info.count = 5;
    info.offset = 0;
    job.setRoomsMembersOrderedByRoleJobInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.membersOrderedByRole?roomId=room_id&offset=0&count=5")));
}

void RoomsMembersOrderedByRoleJobTest::shouldNotStarting()
{
    RoomsMembersOrderedByRoleJob job;

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
    RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo info;
    info.roomId = "room_id"_ba;
    info.count = 0;
    info.offset = 0;
    job.setRoomsMembersOrderedByRoleJobInfo(info);

    QVERIFY(!job.canStart());
    job.setRoomsMembersOrderedByRoleJobInfo(info);
    QVERIFY(!job.canStart());
    info.count = 4;
    job.setRoomsMembersOrderedByRoleJobInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_roomsmembersorderedbyrolejobtest.cpp"
