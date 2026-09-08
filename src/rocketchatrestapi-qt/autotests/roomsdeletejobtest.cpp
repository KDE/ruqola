/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsdeletejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "rooms/roomsdeletejob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RoomsDeleteJobTest)
using namespace RocketChatRestApi;
RoomsDeleteJobTest::RoomsDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsDeleteJobTest::shouldHaveDefaultValue()
{
    RoomsDeleteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoomsDeleteJobTest::shouldGenerateRequest()
{
    RoomsDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoomsDeleteJobTest::shouldGenerateJson()
{
    RoomsDeleteJob job;
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"roomId":"%1"})"_s.arg(roomId).toLatin1());
}

void RoomsDeleteJobTest::shouldNotStarting()
{
    RoomsDeleteJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_roomsdeletejobtest.cpp"
