/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamremoveroomjobtest.h"

#include "ruqola_restapi_helper.h"

#include "teams/teamremoveroomjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(TeamRemoveRoomJobTest)
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
TeamRemoveRoomJobTest::TeamRemoveRoomJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamRemoveRoomJobTest::shouldHaveDefaultValue()
{
    TeamRemoveRoomJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamRemoveRoomJobTest::shouldGenerateRequest()
{
    TeamRemoveRoomJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.removeRoom"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void TeamRemoveRoomJobTest::shouldGenerateJson()
{
    TeamRemoveRoomJob job;
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    const QByteArray teamId("foo2");
    job.setTeamId(teamId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             uR"({"roomId":"%1","teamId":"%2"})"_s.arg(QLatin1StringView(roomId)).arg(QLatin1StringView(teamId)).toLatin1());
}

void TeamRemoveRoomJobTest::shouldNotStarting()
{
    TeamRemoveRoomJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());
    const QByteArray teamId("foo2");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_teamremoveroomjobtest.cpp"
