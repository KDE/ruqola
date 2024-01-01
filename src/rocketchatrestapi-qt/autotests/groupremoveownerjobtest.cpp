/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupremoveownerjobtest.h"
#include "groups/groupremoveownerjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupRemoveOwnerJobTest)
using namespace RocketChatRestApi;
GroupRemoveOwnerJobTest::GroupRemoveOwnerJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupRemoveOwnerJobTest::shouldHaveDefaultValue()
{
    GroupRemoveOwnerJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.removeUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupRemoveOwnerJobTest::shouldGenerateRequest()
{
    GroupRemoveOwnerJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.removeOwner")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void GroupRemoveOwnerJobTest::shouldGenerateJson()
{
    GroupRemoveOwnerJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString removeUserId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setRemoveUserId(removeUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(removeUserId, roomId).toLatin1());
}

#include "moc_groupremoveownerjobtest.cpp"
