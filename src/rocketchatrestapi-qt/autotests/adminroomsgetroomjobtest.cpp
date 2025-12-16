/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsgetroomjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    const QString roomId{u"foo"_s};
    job.setRoomId(roomId);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.adminRooms.getRoom?rid=%1"_s.arg(roomId)));
}

#include "moc_adminroomsgetroomjobtest.cpp"
