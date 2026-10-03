/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomfavoritejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "rooms/roomfavoritejob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RoomFavoriteJobTest)
using namespace RocketChatRestApi;
RoomFavoriteJobTest::RoomFavoriteJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomFavoriteJobTest::shouldHaveDefaultValue()
{
    RoomFavoriteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.favorite());
}

void RoomFavoriteJobTest::shouldGenerateRequest()
{
    RoomFavoriteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.favorite"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoomFavoriteJobTest::shouldGenerateJson()
{
    RoomFavoriteJob job;
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"favorite":true,"roomId":"%1"})"_s.arg(roomId).toLatin1());
}

void RoomFavoriteJobTest::shouldNotStarting()
{
    RoomFavoriteJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_roomfavoritejobtest.cpp"
