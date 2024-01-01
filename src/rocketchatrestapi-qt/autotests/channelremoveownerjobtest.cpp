/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelremoveownerjobtest.h"
#include "channels/channelremoveownerjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChannelRemoveOwnerJobTest)
using namespace RocketChatRestApi;
ChannelRemoveOwnerJobTest::ChannelRemoveOwnerJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelRemoveOwnerJobTest::shouldHaveDefaultValue()
{
    ChannelRemoveOwnerJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.removeUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelRemoveOwnerJobTest::shouldGenerateRequest()
{
    ChannelRemoveOwnerJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.removeOwner")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelRemoveOwnerJobTest::shouldGenerateJson()
{
    ChannelRemoveOwnerJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString removeUserId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setRemoveUserId(removeUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(removeUserId, roomId).toLatin1());
}

#include "moc_channelremoveownerjobtest.cpp"
