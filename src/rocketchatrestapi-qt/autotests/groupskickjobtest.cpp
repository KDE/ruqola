/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupskickjobtest.h"
#include "groups/groupskickjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupsKickJobTest)
using namespace RocketChatRestApi;
GroupsKickJobTest::GroupsKickJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupsKickJobTest::shouldHaveDefaultValue()
{
    GroupsKickJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(job.kickUserId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupsKickJobTest::shouldGenerateRequest()
{
    GroupsKickJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.kick")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void GroupsKickJobTest::shouldGenerateJson()
{
    GroupsKickJob job;
    const QString channelname = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = channelname;
    job.setChannelGroupInfo(info);
    const QString userkick = QStringLiteral("bla");
    job.setKickUserId(userkick);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","userId":"%2"})").arg(channelname, userkick).toLatin1());
}

void GroupsKickJobTest::shouldNotStarting()
{
    GroupsKickJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString roomId = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QVERIFY(!job.canStart());
    const QString kickuser = QStringLiteral("bla");
    job.setKickUserId(kickuser);
    QVERIFY(job.canStart());
}

#include "moc_groupskickjobtest.cpp"
