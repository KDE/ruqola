/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmuteuserjobtest.h"
#include "rooms/roomsmuteuserjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(RoomsMuteUserJobTest)
using namespace RocketChatRestApi;
RoomsMuteUserJobTest::RoomsMuteUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsMuteUserJobTest::shouldHaveDefaultValue()
{
    RoomsMuteUserJob job;
    QVERIFY(job.userName().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoomsMuteUserJobTest::shouldGenerateRequest()
{
    RoomsMuteUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.muteUser")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoomsMuteUserJobTest::shouldGenerateJson()
{
    RoomsMuteUserJob job;
    const QByteArray roomId("room1");
    job.setRoomId(roomId);
    const QString userName(QStringLiteral("usename"));
    job.setUserName(userName);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"roomId":"%1","username":"%2"})").arg(QLatin1StringView(roomId), userName).toLatin1());
}

void RoomsMuteUserJobTest::shouldNotStarting()
{
    RoomsMuteUserJob job;

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

    const QByteArray roomId("room1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());
    const QString userName(QStringLiteral("userName"));
    job.setUserName(userName);
    QVERIFY(job.canStart());
}

#include "moc_roomsmuteuserjobtest.cpp"
