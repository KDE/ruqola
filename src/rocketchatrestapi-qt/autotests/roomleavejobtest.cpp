/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomleavejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "rooms/roomleavejob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RoomLeaveJobTest)
using namespace RocketChatRestApi;
RoomLeaveJobTest::RoomLeaveJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomLeaveJobTest::shouldHaveDefaultValue()
{
    RoomLeaveJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoomLeaveJobTest::shouldGenerateRequest()
{
    RoomLeaveJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.leave"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoomLeaveJobTest::shouldGenerateJson()
{
    RoomLeaveJob job;
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"roomId":"%1"})"_s.arg(roomId).toLatin1());
}

void RoomLeaveJobTest::shouldNotStarting()
{
    RoomLeaveJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_roomleavejobtest.cpp"
