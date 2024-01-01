/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamscreatejobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamscreatejob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TeamsCreateJobTest)
using namespace RocketChatRestApi;
TeamsCreateJobTest::TeamsCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsCreateJobTest::shouldHaveDefaultValue()
{
    TeamsCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    CreateChannelTeamInfo info = job.teamsCreateJobInfo();
    QVERIFY(!info.readOnly);
    QVERIFY(info.name.isEmpty());
    QVERIFY(info.members.isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamsCreateJobTest::shouldGenerateRequest()
{
    TeamsCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TeamsCreateJobTest::shouldGenerateJson()
{
    TeamsCreateJob job;
    CreateChannelTeamInfo info;
    info.infoType = CreateChannelTeamInfo::CreateInfoType::Team;
    const QString channelname = QStringLiteral("foo1");
    info.name = channelname;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1","type":0})").arg(channelname).toLatin1());

    bool readOnly = false;
    info.readOnly = readOnly;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1","type":0})").arg(channelname).toLatin1());

    readOnly = true;
    info.readOnly = readOnly;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1","readOnly":true,"type":0})").arg(channelname).toLatin1());

    const QStringList members = {QStringLiteral("foo"), QStringLiteral("bla")};
    info.members = members;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"extraData":{},"members":["foo","bla"],"name":"%1","readOnly":true,"type":0})").arg(channelname).toLatin1());

    bool privateTeam = true;
    info.privateChannel = privateTeam;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"extraData":{},"members":["foo","bla"],"name":"%1","readOnly":true,"type":1})").arg(channelname).toLatin1());
}

void TeamsCreateJobTest::shouldNotStarting()
{
    TeamsCreateJob job;

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

    CreateChannelTeamInfo info;
    const QString channel = QStringLiteral("foo1");
    info.name = channel;
    job.setTeamsCreateJobInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_teamscreatejobtest.cpp"
