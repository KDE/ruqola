/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeladdmoderatorjobtest.h"
#include "channels/channeladdmoderatorjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChannelAddModeratorJobTest)
using namespace RocketChatRestApi;
ChannelAddModeratorJobTest::ChannelAddModeratorJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelAddModeratorJobTest::shouldHaveDefaultValue()
{
    ChannelAddModeratorJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.addModeratorUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelAddModeratorJobTest::shouldGenerateRequest()
{
    ChannelAddModeratorJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.addModerator")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelAddModeratorJobTest::shouldGenerateJson()
{
    ChannelAddModeratorJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString addUsedId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setAddModeratorUserId(addUsedId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(addUsedId, roomId).toLatin1());
}

#include "moc_channeladdmoderatorjobtest.cpp"
