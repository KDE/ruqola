/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsnameexistsjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    const QString roomName{u"foo"_s};
    job.setRoomName(roomName);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.nameExists?roomName=%1"_s.arg(roomName)));
}

#include "moc_roomsnameexistsjobtest.cpp"
