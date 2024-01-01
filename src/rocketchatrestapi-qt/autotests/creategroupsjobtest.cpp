/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "creategroupsjobtest.h"
#include "groups/creategroupsjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(CreateGroupsJobTest)
using namespace RocketChatRestApi;
CreateGroupsJobTest::CreateGroupsJobTest(QObject *parent)
    : QObject(parent)
{
}

void CreateGroupsJobTest::shouldHaveDefaultValue()
{
    CreateGroupsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    CreateChannelTeamInfo info = job.createGroupsInfo();
    QVERIFY(!info.readOnly);
    QVERIFY(info.name.isEmpty());
    QVERIFY(info.members.isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CreateGroupsJobTest::shouldGenerateRequest()
{
    CreateGroupsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void CreateGroupsJobTest::shouldGenerateJson()
{
    CreateGroupsJob job;
    const QString channelname = QStringLiteral("foo1");
    CreateChannelTeamInfo info;
    info.name = channelname;
    job.setCreateGroupsInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1"})").arg(channelname).toLatin1());

    bool readOnly = false;
    info.readOnly = readOnly;
    job.setCreateGroupsInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1"})").arg(channelname).toLatin1());

    readOnly = true;
    info.readOnly = readOnly;
    job.setCreateGroupsInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1","readOnly":true})").arg(channelname).toLatin1());

    const QStringList members = {QStringLiteral("foo"), QStringLiteral("bla")};
    info.members = members;
    job.setCreateGroupsInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"extraData":{},"members":["foo","bla"],"name":"%1","readOnly":true})").arg(channelname).toLatin1());

    const QString teamId = {QStringLiteral("foo")};
    info.teamId = teamId;
    info.broadcast = true;
    job.setCreateGroupsInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(R"({"extraData":{"broadcast":true,"teamId":"foo"},"members":["foo","bla"],"name":"%1","readOnly":true})").arg(channelname).toLatin1());
}

void CreateGroupsJobTest::shouldNotStarting()
{
    CreateGroupsJob job;

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
    CreateChannelTeamInfo info;
    info.name = roomId;
    job.setCreateGroupsInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_creategroupsjobtest.cpp"
