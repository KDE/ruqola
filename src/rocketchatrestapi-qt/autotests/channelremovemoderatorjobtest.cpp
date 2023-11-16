/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelremovemoderatorjobtest.h"
#include "channels/channelremovemoderatorjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChannelRemoveModeratorJobTest)
using namespace RocketChatRestApi;
ChannelRemoveModeratorJobTest::ChannelRemoveModeratorJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelRemoveModeratorJobTest::shouldHaveDefaultValue()
{
    ChannelRemoveModeratorJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.removeUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelRemoveModeratorJobTest::shouldGenerateRequest()
{
    ChannelRemoveModeratorJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.removeModerator")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelRemoveModeratorJobTest::shouldGenerateJson()
{
    ChannelRemoveModeratorJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString removeUserId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setRemoveUserId(removeUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(removeUserId, roomId).toLatin1());
}

#include "moc_channelremovemoderatorjobtest.cpp"
