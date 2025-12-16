/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmuteuserjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rooms/roomsmuteuserjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoomsMuteUserJobTest::shouldGenerateRequest()
{
    RoomsMuteUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.muteUser"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoomsMuteUserJobTest::shouldGenerateJson()
{
    RoomsMuteUserJob job;
    const QByteArray roomId("room1");
    job.setRoomId(roomId);
    const QString userName(u"usename"_s);
    job.setUserName(userName);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"roomId":"%1","username":"%2"})").arg(QLatin1StringView(roomId), userName).toLatin1());
}

void RoomsMuteUserJobTest::shouldNotStarting()
{
    RoomsMuteUserJob job;

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

    const QByteArray roomId("room1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());
    const QString userName(u"userName"_s);
    job.setUserName(userName);
    QVERIFY(job.canStart());
}

#include "moc_roomsmuteuserjobtest.cpp"
