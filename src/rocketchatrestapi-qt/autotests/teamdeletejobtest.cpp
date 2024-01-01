/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamdeletejobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamdeletejob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TeamDeleteJobTest)
using namespace RocketChatRestApi;
TeamDeleteJobTest::TeamDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamDeleteJobTest::shouldHaveDefaultValue()
{
    TeamDeleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomsId().isEmpty());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamDeleteJobTest::shouldGenerateRequest()
{
    TeamDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TeamDeleteJobTest::shouldGenerateJson()
{
    TeamDeleteJob job;
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"teamId":"%1"})").arg(teamId).toLatin1());
    const QStringList rooms = {QStringLiteral("bla"), QStringLiteral("bla1")};
    job.setRoomsId(rooms);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomsToRemove":["bla","bla1"],"teamId":"%1"})").arg(teamId).toLatin1());
}

void TeamDeleteJobTest::shouldNotStarting()
{
    TeamDeleteJob job;

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
    QVERIFY(!job.canStart());
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
    // roomsId can be empty
    const QStringList rooms = {QStringLiteral("bb"), QStringLiteral("aa")};
    job.setRoomsId(rooms);
    QVERIFY(job.canStart());
}

#include "moc_teamdeletejobtest.cpp"
