/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsconverttoteamjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/channelsconverttoteamjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChannelsConvertToTeamJobTest)
using namespace RocketChatRestApi;
ChannelsConvertToTeamJobTest::ChannelsConvertToTeamJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelsConvertToTeamJobTest::shouldHaveDefaultValue()
{
    ChannelsConvertToTeamJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.channelId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelsConvertToTeamJobTest::shouldGenerateRequest()
{
    ChannelsConvertToTeamJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.convertToTeam")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelsConvertToTeamJobTest::shouldGenerateJson()
{
    ChannelsConvertToTeamJob job;
    const QString channelId = QStringLiteral("foo2");
    job.setChannelId(channelId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"channelId":"%1"})").arg(channelId).toLatin1());
}

void ChannelsConvertToTeamJobTest::shouldNotStarting()
{
    ChannelsConvertToTeamJob job;

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
    job.setChannelId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_channelsconverttoteamjobtest.cpp"
