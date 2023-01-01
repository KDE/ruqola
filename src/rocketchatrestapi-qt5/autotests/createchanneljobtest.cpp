/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createchanneljobtest.h"
#include "channels/createchanneljob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(CreateChannelJobTest)
using namespace RocketChatRestApi;
CreateChannelJobTest::CreateChannelJobTest(QObject *parent)
    : QObject(parent)
{
}

void CreateChannelJobTest::shouldHaveDefaultValue()
{
    CreateChannelJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    CreateChannelTeamInfo info = job.createChannelInfo();
    QVERIFY(!info.readOnly);
    QVERIFY(info.name.isEmpty());
    QVERIFY(info.members.isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CreateChannelJobTest::shouldGenerateRequest()
{
    CreateChannelJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void CreateChannelJobTest::shouldGenerateJson()
{
    CreateChannelJob job;
    const QString channelname = QStringLiteral("foo1");
    CreateChannelTeamInfo info;
    info.name = channelname;
    job.setCreateChannelInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1"})").arg(channelname).toLatin1());

    bool readOnly = false;
    info.readOnly = readOnly;
    job.setCreateChannelInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1"})").arg(channelname).toLatin1());

    readOnly = true;
    info.readOnly = readOnly;
    job.setCreateChannelInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"extraData":{},"name":"%1","readOnly":true})").arg(channelname).toLatin1());

    const QStringList members = {QStringLiteral("foo"), QStringLiteral("bla")};
    info.members = members;
    job.setCreateChannelInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"extraData":{},"members":["foo","bla"],"name":"%1","readOnly":true})").arg(channelname).toLatin1());

    const QString teamId = {QStringLiteral("foo")};
    info.teamId = teamId;
    job.setCreateChannelInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"extraData":{"teamId":"foo"},"members":["foo","bla"],"name":"%1","readOnly":true})").arg(channelname).toLatin1());
}

void CreateChannelJobTest::shouldNotStarting()
{
    CreateChannelJob job;

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
    const QString channelName = QStringLiteral("foo1");
    CreateChannelTeamInfo info;
    info.name = channelName;
    job.setCreateChannelInfo(info);
    QVERIFY(job.canStart());
}
