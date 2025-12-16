/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamremovemembersjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include "teams/teamremovemembersjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(TeamRemoveMembersJobTest)
using namespace RocketChatRestApi;
TeamRemoveMembersJobTest::TeamRemoveMembersJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamRemoveMembersJobTest::shouldHaveDefaultValue()
{
    TeamRemoveMembersJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomsId().isEmpty());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamRemoveMembersJobTest::shouldGenerateRequest()
{
    TeamRemoveMembersJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.removeMember"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void TeamRemoveMembersJobTest::shouldGenerateJson()
{
    TeamRemoveMembersJob job;
    const QString teamId = u"foo2"_s;
    job.setTeamId(teamId);
    const QStringList rooms = {u"bla"_s, u"bla1"_s};
    job.setRoomsId(rooms);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"rooms":["bla","bla1"],"teamId":"%1"})").arg(teamId).toLatin1());
}

void TeamRemoveMembersJobTest::shouldNotStarting()
{
    TeamRemoveMembersJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    QVERIFY(!job.canStart());
    const QString teamId = u"foo2"_s;
    job.setTeamId(teamId);
    const QStringList rooms = {u"bb"_s, u"aa"_s};
    job.setRoomsId(rooms);
    QVERIFY(job.canStart());
}

#include "moc_teamremovemembersjobtest.cpp"
