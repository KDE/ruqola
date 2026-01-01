/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    const QString roomId{u"fooid"_s};
    job.setRoomId(roomId);

    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.info?roomId=%1"_s.arg(roomId)));
}

void RoomsInfoJobTest::shouldNotStarting()
{
    RoomsInfoJob job;

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
    const QString roomId{u"fooid"_s};
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_roomsinfojobtest.cpp"
