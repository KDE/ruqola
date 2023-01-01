/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeladdleaderjobtest.h"
#include "channels/channeladdleaderjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChannelAddLeaderJobTest)
using namespace RocketChatRestApi;
ChannelAddLeaderJobTest::ChannelAddLeaderJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelAddLeaderJobTest::shouldHaveDefaultValue()
{
    ChannelAddLeaderJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.addLeaderUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelAddLeaderJobTest::shouldGenerateRequest()
{
    ChannelAddLeaderJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.addLeader")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelAddLeaderJobTest::shouldGenerateJson()
{
    ChannelAddLeaderJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString removeUserId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setAddLeaderUserId(removeUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(removeUserId, roomId).toLatin1());
}
