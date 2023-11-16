/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamremovemembersjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamremovemembersjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TeamRemoveMembersJobTest)
using namespace RocketChatRestApi;
TeamRemoveMembersJobTest::TeamRemoveMembersJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamRemoveMembersJobTest::shouldHaveDefaultValue()
{
    TeamRemoveMembersJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomsId().isEmpty());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamRemoveMembersJobTest::shouldGenerateRequest()
{
    TeamRemoveMembersJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.removeMember")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TeamRemoveMembersJobTest::shouldGenerateJson()
{
    TeamRemoveMembersJob job;
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);
    const QStringList rooms = {QStringLiteral("bla"), QStringLiteral("bla1")};
    job.setRoomsId(rooms);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"rooms":["bla","bla1"],"teamId":"%1"})").arg(teamId).toLatin1());
}

void TeamRemoveMembersJobTest::shouldNotStarting()
{
    TeamRemoveMembersJob job;

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
    const QStringList rooms = {QStringLiteral("bb"), QStringLiteral("aa")};
    job.setRoomsId(rooms);
    QVERIFY(job.canStart());
}

#include "moc_teamremovemembersjobtest.cpp"
