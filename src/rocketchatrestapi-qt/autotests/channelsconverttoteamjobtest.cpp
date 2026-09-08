/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsconverttoteamjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include "teams/channelsconverttoteamjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ChannelsConvertToTeamJobTest)
using namespace RocketChatRestApi;
ChannelsConvertToTeamJobTest::ChannelsConvertToTeamJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelsConvertToTeamJobTest::shouldHaveDefaultValue()
{
    ChannelsConvertToTeamJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.channelId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelsConvertToTeamJobTest::shouldGenerateRequest()
{
    ChannelsConvertToTeamJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.convertToTeam"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChannelsConvertToTeamJobTest::shouldGenerateJson()
{
    ChannelsConvertToTeamJob job;
    const QString channelId = u"foo2"_s;
    job.setChannelId(channelId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"channelId":"%1"})"_s.arg(channelId).toLatin1());
}

void ChannelsConvertToTeamJobTest::shouldNotStarting()
{
    ChannelsConvertToTeamJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString teamId = u"foo2"_s;
    job.setChannelId(teamId);
    QVERIFY(job.canStart());
}

#include "moc_channelsconverttoteamjobtest.cpp"
