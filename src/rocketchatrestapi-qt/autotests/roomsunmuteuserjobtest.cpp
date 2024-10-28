/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsunmuteuserjobtest.h"
#include "rooms/roomsunmuteuserjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(RoomsUnmuteUserJobTest)
using namespace RocketChatRestApi;
RoomsUnmuteUserJobTest::RoomsUnmuteUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsUnmuteUserJobTest::shouldHaveDefaultValue()
{
    RoomsUnmuteUserJob job;
    QVERIFY(job.userName().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoomsUnmuteUserJobTest::shouldGenerateRequest()
{
    RoomsUnmuteUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.unmuteUser")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoomsUnmuteUserJobTest::shouldGenerateJson()
{
    RoomsUnmuteUserJob job;
    // TODO
}

void RoomsUnmuteUserJobTest::shouldNotStarting()
{
    RoomsUnmuteUserJob job;

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

#include "moc_roomsunmuteuserjobtest.cpp"
