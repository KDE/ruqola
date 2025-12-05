/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsannouncementjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.announcement().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsAnnouncementJobTest::shouldGenerateRequest()
{
    ChangeGroupsAnnouncementJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.setAnnouncement"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeGroupsAnnouncementJobTest::shouldGenerateJson()
{
    ChangeGroupsAnnouncementJob job;
    const QString roomId = u"foo1"_s;
    const QString announcement = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);

    job.setAnnouncement(announcement);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"announcement":"%1","roomId":"%2"})").arg(announcement, roomId).toLatin1());
}

#include "moc_changegroupsannouncementjobtest.cpp"
