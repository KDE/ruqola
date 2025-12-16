/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupstopicjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/changegroupstopicjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ChangeGroupsTopicJobTest)
using namespace RocketChatRestApi;
ChangeGroupsTopicJobTest::ChangeGroupsTopicJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsTopicJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsTopicJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.topic().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsTopicJobTest::shouldGenerateRequest()
{
    ChangeGroupsTopicJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.setTopic"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeGroupsTopicJobTest::shouldGenerateJson()
{
    ChangeGroupsTopicJob job;
    const QString roomId = u"foo1"_s;
    const QString topic = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setTopic(topic);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","topic":"%2"})").arg(roomId, topic).toLatin1());
}

#include "moc_changegroupstopicjobtest.cpp"
