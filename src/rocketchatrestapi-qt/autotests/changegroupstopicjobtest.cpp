/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupstopicjobtest.h"
#include "groups/changegroupstopicjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeGroupsTopicJobTest)
using namespace RocketChatRestApi;
ChangeGroupsTopicJobTest::ChangeGroupsTopicJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsTopicJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsTopicJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.topic().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsTopicJobTest::shouldGenerateRequest()
{
    ChangeGroupsTopicJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setTopic")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeGroupsTopicJobTest::shouldGenerateJson()
{
    ChangeGroupsTopicJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString topic = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setTopic(topic);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","topic":"%2"})").arg(roomId, topic).toLatin1());
}

#include "moc_changegroupstopicjobtest.cpp"
