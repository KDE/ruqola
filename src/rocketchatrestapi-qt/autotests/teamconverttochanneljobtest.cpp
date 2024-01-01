/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamconverttochanneljobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamconverttochanneljob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TeamConvertToChannelJobTest)
using namespace RocketChatRestApi;
TeamConvertToChannelJobTest::TeamConvertToChannelJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamConvertToChannelJobTest::shouldHaveDefaultValue()
{
    TeamConvertToChannelJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(job.roomsToRemove().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamConvertToChannelJobTest::shouldGenerateRequest()
{
    TeamConvertToChannelJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.convertToChannel")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TeamConvertToChannelJobTest::shouldGenerateJson()
{
    TeamConvertToChannelJob job;
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"teamId":"%1"})").arg(teamId).toLatin1());
    const QStringList roomsToRemove{QStringLiteral("bla"), QStringLiteral("bla1"), QStringLiteral("bla2")};
    job.setRoomsToRemove(roomsToRemove);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomsToRemove":["bla","bla1","bla2"],"teamId":"%1"})").arg(teamId).toLatin1());
}

void TeamConvertToChannelJobTest::shouldNotStarting()
{
    TeamConvertToChannelJob job;

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
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_teamconverttochanneljobtest.cpp"
