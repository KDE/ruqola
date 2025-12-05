/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "creategroupsjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.create"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void CreateGroupsJobTest::shouldGenerateJson()
{
    CreateGroupsJob job;
    const QString channelname = u"foo1"_s;
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

    const QStringList members = {u"foo"_s, u"bla"_s};
    info.members = members;
    job.setCreateGroupsInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"extraData":{},"members":["foo","bla"],"name":"%1","readOnly":true})").arg(channelname).toLatin1());

    const QByteArray teamId("foo");
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
    const QString roomId = u"foo1"_s;
    CreateChannelTeamInfo info;
    info.name = roomId;
    job.setCreateGroupsInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_creategroupsjobtest.cpp"
