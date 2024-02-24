/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsnameexistsjobtest.h"
#include "restapimethod.h"
#include "rooms/roomsnameexistsjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomsNameExistsJobTest)
using namespace RocketChatRestApi;
RoomsNameExistsJobTest::RoomsNameExistsJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsNameExistsJobTest::shouldHaveDefaultValue()
{
    RoomsNameExistsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.roomName().isEmpty());
}

void RoomsNameExistsJobTest::shouldGenerateRequest()
{
    RoomsNameExistsJob job;
    const QString roomName{QStringLiteral("foo")};
    job.setRoomName(roomName);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.nameExists?roomName=%1").arg(roomName)));
}

#include "moc_roomsnameexistsjobtest.cpp"
