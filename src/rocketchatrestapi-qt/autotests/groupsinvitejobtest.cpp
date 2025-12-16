/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsinvitejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/groupsinvitejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(GroupsInviteJobTest)
using namespace RocketChatRestApi;
GroupsInviteJobTest::GroupsInviteJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupsInviteJobTest::shouldHaveDefaultValue()
{
    GroupsInviteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.inviteUserId().isEmpty());
    QVERIFY(job.inviteUserName().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupsInviteJobTest::shouldGenerateRequest()
{
    GroupsInviteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.invite"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void GroupsInviteJobTest::shouldGenerateUsernameJson()
{
    GroupsInviteJob job;
    const QString roomId = u"foo1"_s;
    const QString userId = u"topic1"_s;
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
    const QString roomId = u"foo1"_s;
    const QString userId = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setInviteUserId(userId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(userId, roomId).toLatin1());
}

#include "moc_groupsinvitejobtest.cpp"
