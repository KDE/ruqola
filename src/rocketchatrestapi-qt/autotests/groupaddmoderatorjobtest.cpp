/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupaddmoderatorjobtest.h"
#include "groups/groupaddmoderatorjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupAddModeratorJobTest)
using namespace RocketChatRestApi;
GroupAddModeratorJobTest::GroupAddModeratorJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupAddModeratorJobTest::shouldHaveDefaultValue()
{
    GroupAddModeratorJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.addModeratorUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupAddModeratorJobTest::shouldGenerateRequest()
{
    GroupAddModeratorJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.addModerator")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void GroupAddModeratorJobTest::shouldGenerateJson()
{
    GroupAddModeratorJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString addUsedId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job.setChannelGroupInfo(info);
    job.setAddModeratorUserId(addUsedId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(addUsedId, roomId).toLatin1());
}

#include "moc_groupaddmoderatorjobtest.cpp"
