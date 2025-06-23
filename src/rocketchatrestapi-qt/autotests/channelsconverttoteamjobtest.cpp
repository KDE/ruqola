/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsconverttoteamjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.convertToTeam"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChannelsConvertToTeamJobTest::shouldGenerateJson()
{
    ChannelsConvertToTeamJob job;
    const QString channelId = u"foo2"_s;
    job.setChannelId(channelId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"channelId":"%1"})").arg(channelId).toLatin1());
}

void ChannelsConvertToTeamJobTest::shouldNotStarting()
{
    ChannelsConvertToTeamJob job;

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
    const QString teamId = u"foo2"_s;
    job.setChannelId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_channelsconverttoteamjobtest.cpp"
