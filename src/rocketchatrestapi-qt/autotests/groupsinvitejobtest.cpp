/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsinvitejobtest.h"
#include "groups/groupsinvitejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupsInviteJobTest)
using namespace RocketChatRestApi;
GroupsInviteJobTest::GroupsInviteJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupsInviteJobTest::shouldHaveDefaultValue()
{
    GroupsInviteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.inviteUserId().isEmpty());
    QVERIFY(job.inviteUserName().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupsInviteJobTest::shouldGenerateRequest()
{
    GroupsInviteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.invite")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void GroupsInviteJobTest::shouldGenerateUsernameJson()
{
    GroupsInviteJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString userId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setInviteUserName(userId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userName":"%1"})").arg(userId, roomId).toLatin1());
}

void GroupsInviteJobTest::shouldGenerateUserIdJson()
{
    GroupsInviteJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString userId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setInviteUserId(userId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(userId, roomId).toLatin1());
}

#include "moc_groupsinvitejobtest.cpp"
