/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsimagesjobtest.h"
#include "restapimethod.h"
#include "rooms/getdiscussionsjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomsImagesJobTest)
using namespace RocketChatRestApi;
RoomsImagesJobTest::RoomsImagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsImagesJobTest::shouldHaveDefaultValue()
{
    GetDiscussionsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoomsImagesJobTest::shouldGenerateRequest()
{
    GetDiscussionsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QByteArray roomId("bla");
    job.setRoomId(roomId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.getDiscussions?roomId=%1").arg(QLatin1StringView(roomId))));
}

void RoomsImagesJobTest::shouldNotStarting()
{
    GetDiscussionsJob job;

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
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_roomsimagesjobtest.cpp"
