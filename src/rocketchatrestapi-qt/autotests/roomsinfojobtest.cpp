/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsinfojobtest.h"
#include "restapimethod.h"
#include "rooms/roomsinfojob.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomsInfoJobTest)
using namespace RocketChatRestApi;
RoomsInfoJobTest::RoomsInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsInfoJobTest::shouldHaveDefaultValue()
{
    RoomsInfoJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.roomId().isEmpty());
}

void RoomsInfoJobTest::shouldGenerateRequest()
{
    RoomsInfoJob job;
    RestApiMethod method;
    const QString roomId{QStringLiteral("fooid")};
    job.setRoomId(roomId);

    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.info?roomId=%1").arg(roomId)));
}

void RoomsInfoJobTest::shouldNotStarting()
{
    RoomsInfoJob job;

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
    const QString roomId{QStringLiteral("fooid")};
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_roomsinfojobtest.cpp"
