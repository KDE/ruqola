/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechanneltopicjobtest.h"
#include "channels/changechanneltopicjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeChannelTopicJobTest)
using namespace RocketChatRestApi;
ChangeChannelTopicJobTest::ChangeChannelTopicJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelTopicJobTest::shouldHaveDefaultValue()
{
    ChangeChannelTopicJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.topic().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelTopicJobTest::shouldGenerateRequest()
{
    ChangeChannelTopicJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setTopic")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeChannelTopicJobTest::shouldGenerateJson()
{
    ChangeChannelTopicJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString topic = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setTopic(topic);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","topic":"%2"})").arg(roomId, topic).toLatin1());
}

#include "moc_changechanneltopicjobtest.cpp"
