/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupskickjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/groupskickjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(GroupsKickJobTest)
using namespace RocketChatRestApi;
GroupsKickJobTest::GroupsKickJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupsKickJobTest::shouldHaveDefaultValue()
{
    GroupsKickJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(job.kickUserId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupsKickJobTest::shouldGenerateRequest()
{
    GroupsKickJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.kick"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void GroupsKickJobTest::shouldGenerateJson()
{
    GroupsKickJob job;
    const QString channelname = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    info.identifier = channelname;
    job.setChannelGroupInfo(info);
    const QByteArray userkick("bla");
    job.setKickUserId(userkick);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"roomId":"%1","userId":"%2"})"_s.arg(channelname, QLatin1StringView(userkick)).toLatin1());
}

void GroupsKickJobTest::shouldNotStarting()
{
    GroupsKickJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QVERIFY(!job.canStart());
    const QByteArray kickuser("bla");
    job.setKickUserId(kickuser);
    QVERIFY(job.canStart());
}

#include "moc_groupskickjobtest.cpp"
