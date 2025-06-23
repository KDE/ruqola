/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsimagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QByteArray roomId("bla");
    RoomsImagesJob::RoomsImagesJobInfo info;
    info.roomId = "room_id"_ba;
    info.startingFromId = "start_Id"_ba;
    info.count = 5;
    info.offset = 0;
    job.setRoomsImagesJobInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.images?roomId=room_id&startingFromId=start_Id&offset=0&count=5"_s));
}

void RoomsImagesJobTest::shouldNotStarting()
{
    RoomsImagesJob job;

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
    RoomsImagesJob::RoomsImagesJobInfo info;
    info.roomId = "room_id"_ba;
    info.startingFromId = "start_id"_ba;
    info.count = 0;
    info.offset = 0;
    job.setRoomsImagesJobInfo(info);

    QVERIFY(!job.canStart());
    info.startingFromId = "room33"_ba;
    job.setRoomsImagesJobInfo(info);
    QVERIFY(!job.canStart());
    info.count = 4;
    job.setRoomsImagesJobInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_roomsimagesjobtest.cpp"
