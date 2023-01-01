/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamaddroomsjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamaddroomsjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(TeamAddRoomsJobTest)
using namespace RocketChatRestApi;
TeamAddRoomsJobTest::TeamAddRoomsJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamAddRoomsJobTest::shouldHaveDefaultValue()
{
    TeamAddRoomsJob job;
    verifyDefaultValue(&job);
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
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.addRooms")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TeamAddRoomsJobTest::shouldGenerateJson()
{
    TeamAddRoomsJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomIds({roomId});
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"rooms":["%1"],"teamId":"%2"})").arg(roomId, teamId).toLatin1());
}

void TeamAddRoomsJobTest::shouldNotStarting()
{
    TeamAddRoomsJob job;

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
    const QString roomId = QStringLiteral("foo1");
    job.setRoomIds({roomId});
    QVERIFY(!job.canStart());
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}
