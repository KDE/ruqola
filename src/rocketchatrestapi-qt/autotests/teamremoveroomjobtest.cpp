/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamremoveroomjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamremoveroomjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TeamRemoveRoomJobTest)
using namespace RocketChatRestApi;
TeamRemoveRoomJobTest::TeamRemoveRoomJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamRemoveRoomJobTest::shouldHaveDefaultValue()
{
    TeamRemoveRoomJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamRemoveRoomJobTest::shouldGenerateRequest()
{
    TeamRemoveRoomJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.removeRoom")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TeamRemoveRoomJobTest::shouldGenerateJson()
{
    TeamRemoveRoomJob job;
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    const QByteArray teamId("foo2");
    job.setTeamId(teamId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"roomId":"%1","teamId":"%2"})").arg(QLatin1StringView(roomId)).arg(QLatin1StringView(teamId)).toLatin1());
}

void TeamRemoveRoomJobTest::shouldNotStarting()
{
    TeamRemoveRoomJob job;

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
    QVERIFY(!job.canStart());
    const QByteArray teamId("foo2");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_teamremoveroomjobtest.cpp"
