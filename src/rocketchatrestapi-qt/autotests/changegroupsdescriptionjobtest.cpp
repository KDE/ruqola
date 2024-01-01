/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsdescriptionjobtest.h"
#include "groups/changegroupsdescriptionjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeGroupsDescriptionJobTest)
using namespace RocketChatRestApi;
ChangeGroupsDescriptionJobTest::ChangeGroupsDescriptionJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsDescriptionJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsDescriptionJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.description().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsDescriptionJobTest::shouldGenerateRequest()
{
    ChangeGroupsDescriptionJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setDescription")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeGroupsDescriptionJobTest::shouldGenerateJson()
{
    ChangeGroupsDescriptionJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString description = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setDescription(description);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"description":"%1","roomId":"%2"})").arg(description, roomId).toLatin1());
}

#include "moc_changegroupsdescriptionjobtest.cpp"
