/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinvitejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelinvitejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ChannelInviteJobTest)
using namespace RocketChatRestApi;
ChannelInviteJobTest::ChannelInviteJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInviteJobTest::shouldHaveDefaultValue()
{
    ChannelInviteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.inviteUserId().isEmpty());
    QVERIFY(job.inviteUserName().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelInviteJobTest::shouldGenerateRequest()
{
    ChannelInviteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.invite"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChannelInviteJobTest::shouldGenerateUserIdJson()
{
    ChannelInviteJob job;
    const QString roomId = u"foo1"_s;
    const QString userId = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setInviteUserId(userId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(userId, roomId).toLatin1());
}

void ChannelInviteJobTest::shouldGenerateUserNameJson()
{
    ChannelInviteJob job;
    const QString roomId = u"foo1"_s;
    const QString userName = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setInviteUserName(userName);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userName":"%1"})").arg(userName, roomId).toLatin1());
}

#include "moc_channelinvitejobtest.cpp"
