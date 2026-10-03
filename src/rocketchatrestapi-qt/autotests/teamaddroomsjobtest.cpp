/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamaddroomsjobtest.h"

#include "ruqola_restapi_helper.h"

#include "teams/teamaddroomsjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(TeamAddRoomsJobTest)
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
TeamAddRoomsJobTest::TeamAddRoomsJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamAddRoomsJobTest::shouldHaveDefaultValue()
{
    TeamAddRoomsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomIds().isEmpty());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void TeamAddRoomsJobTest::shouldGenerateRequest()
{
    TeamAddRoomsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.addRooms"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void TeamAddRoomsJobTest::shouldGenerateJson()
{
    TeamAddRoomsJob job;
    const QByteArray roomId("foo1");
    job.setRoomIds({roomId});
    const QByteArray teamId("foo2");
    job.setTeamId(teamId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             uR"({"rooms":["%1"],"teamId":"%2"})"_s.arg(QLatin1StringView(roomId), QLatin1StringView(teamId)).toLatin1());
}

void TeamAddRoomsJobTest::shouldNotStarting()
{
    TeamAddRoomsJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray roomId("foo1");
    job.setRoomIds({roomId});
    QVERIFY(!job.canStart());
    const QByteArray teamId("foo2");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_teamaddroomsjobtest.cpp"
