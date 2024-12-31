/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsimagesjobtest.h"
#include "restapimethod.h"
#include "rooms/roomsimagesjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomsImagesJobTest)
using namespace RocketChatRestApi;
RoomsImagesJobTest::RoomsImagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsImagesJobTest::shouldHaveDefaultValue()
{
    RoomsImagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.roomsImagesJobInfo().isValid());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoomsImagesJobTest::shouldGenerateRequest()
{
    RoomsImagesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QByteArray roomId("bla");
    RoomsImagesJob::RoomsImagesJobInfo info;
    info.roomId = QByteArrayLiteral("room_id");
    info.startingFromId = QByteArrayLiteral("start_Id");
    info.count = 5;
    info.offset = 0;
    job.setRoomsImagesJobInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.images?roomId=room_id&startingFromId=start_Id&offset=0&count=5")));
}

void RoomsImagesJobTest::shouldNotStarting()
{
    RoomsImagesJob job;

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
    RoomsImagesJob::RoomsImagesJobInfo info;
    info.roomId = QByteArrayLiteral("room_id");
    info.startingFromId;
    info.count = 0;
    info.offset = 0;
    job.setRoomsImagesJobInfo(info);

    QVERIFY(!job.canStart());
    info.startingFromId = QByteArrayLiteral("room33");
    job.setRoomsImagesJobInfo(info);
    QVERIFY(!job.canStart());
    info.count = 4;
    job.setRoomsImagesJobInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_roomsimagesjobtest.cpp"
