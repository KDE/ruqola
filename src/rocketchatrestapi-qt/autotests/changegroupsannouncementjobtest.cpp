/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsannouncementjobtest.h"
#include "groups/changegroupsannouncementjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeGroupsAnnouncementJobTest)
using namespace RocketChatRestApi;
ChangeGroupsAnnouncementJobTest::ChangeGroupsAnnouncementJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsAnnouncementJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsAnnouncementJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.announcement().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsAnnouncementJobTest::shouldGenerateRequest()
{
    ChangeGroupsAnnouncementJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setAnnouncement")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeGroupsAnnouncementJobTest::shouldGenerateJson()
{
    ChangeGroupsAnnouncementJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString announcement = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);

    job.setAnnouncement(announcement);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"announcement":"%1","roomId":"%2"})").arg(announcement, roomId).toLatin1());
}

#include "moc_changegroupsannouncementjobtest.cpp"
