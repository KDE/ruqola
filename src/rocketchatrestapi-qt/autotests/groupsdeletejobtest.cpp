/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsdeletejobtest.h"
#include "groups/groupsdeletejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupsDeleteJobTest)
using namespace RocketChatRestApi;
GroupsDeleteJobTest::GroupsDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupsDeleteJobTest::shouldHaveDefaultValue()
{
    GroupsDeleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupsDeleteJobTest::shouldGenerateRequest()
{
    GroupsDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void GroupsDeleteJobTest::shouldGenerateRoomIdJson()
{
    GroupsDeleteJob job;
    const QString roomId = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job.setChannelGroupInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

#include "moc_groupsdeletejobtest.cpp"
