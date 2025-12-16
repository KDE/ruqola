/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechanneltopicjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/changechanneltopicjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeChannelTopicJobTest)
using namespace RocketChatRestApi;
ChangeChannelTopicJobTest::ChangeChannelTopicJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelTopicJobTest::shouldHaveDefaultValue()
{
    ChangeChannelTopicJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.topic().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelTopicJobTest::shouldGenerateRequest()
{
    ChangeChannelTopicJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.setTopic"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeChannelTopicJobTest::shouldGenerateJson()
{
    ChangeChannelTopicJob job;
    const QString roomId = u"foo1"_s;
    const QString topic = u"topic1"_s;
    job.setRoomId(roomId);
    job.setTopic(topic);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","topic":"%2"})").arg(roomId, topic).toLatin1());
}

#include "moc_changechanneltopicjobtest.cpp"
