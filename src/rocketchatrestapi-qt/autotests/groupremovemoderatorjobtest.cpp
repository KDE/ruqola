/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupremovemoderatorjobtest.h"
#include "groups/groupremovemoderatorjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupRemoveModeratorJobTest)
using namespace RocketChatRestApi;
GroupRemoveModeratorJobTest::GroupRemoveModeratorJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupRemoveModeratorJobTest::shouldHaveDefaultValue()
{
    GroupRemoveModeratorJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.removeUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupRemoveModeratorJobTest::shouldGenerateRequest()
{
    GroupRemoveModeratorJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.removeModerator")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void GroupRemoveModeratorJobTest::shouldGenerateJson()
{
    GroupRemoveModeratorJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString removeUserId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job.setChannelGroupInfo(info);
    job.setRemoveUserId(removeUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(removeUserId, roomId).toLatin1());
}

#include "moc_groupremovemoderatorjobtest.cpp"
