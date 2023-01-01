/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsgetroomjobtest.h"
#include "restapimethod.h"
#include "rooms/adminroomsgetroomjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(AdminRoomsGetRoomJobTest)
using namespace RocketChatRestApi;
AdminRoomsGetRoomJobTest::AdminRoomsGetRoomJobTest(QObject *parent)
    : QObject(parent)
{
}

void AdminRoomsGetRoomJobTest::shouldHaveDefaultValue()
{
    AdminRoomsGetRoomJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.roomId().isEmpty());
}

void AdminRoomsGetRoomJobTest::shouldGenerateRequest()
{
    AdminRoomsGetRoomJob job;
    const QString roomId{QStringLiteral("foo")};
    job.setRoomId(roomId);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.adminRooms.getRoom?rid=%1").arg(roomId)));
}
