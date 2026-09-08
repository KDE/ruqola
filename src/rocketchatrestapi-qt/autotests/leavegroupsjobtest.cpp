/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "leavegroupsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/leavegroupsjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(LeaveGroupsJobTest)
using namespace RocketChatRestApi;
LeaveGroupsJobTest::LeaveGroupsJobTest(QObject *parent)
    : QObject(parent)
{
}

void LeaveGroupsJobTest::shouldHaveDefaultValue()
{
    LeaveGroupsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void LeaveGroupsJobTest::shouldGenerateRequest()
{
    LeaveGroupsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.leave"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void LeaveGroupsJobTest::shouldGenerateJson()
{
    LeaveGroupsJob job;
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    job.setChannelGroupInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"roomId":"%1"})"_s.arg(roomId).toLatin1());
}

void LeaveGroupsJobTest::shouldNotStarting()
{
    LeaveGroupsJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    job.setChannelGroupInfo(info);

    QVERIFY(job.canStart());
}

#include "moc_leavegroupsjobtest.cpp"
