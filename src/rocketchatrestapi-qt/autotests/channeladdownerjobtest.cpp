/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeladdownerjobtest.h"
#include "channels/channeladdownerjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChannelAddOwnerJobTest)
using namespace RocketChatRestApi;
ChannelAddOwnerJobTest::ChannelAddOwnerJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelAddOwnerJobTest::shouldHaveDefaultValue()
{
    ChannelAddOwnerJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.addownerUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelAddOwnerJobTest::shouldGenerateRequest()
{
    ChannelAddOwnerJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.addOwner")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelAddOwnerJobTest::shouldGenerateJson()
{
    ChannelAddOwnerJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString addUsedId = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setAddownerUserId(addUsedId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(addUsedId, roomId).toLatin1());
}

#include "moc_channeladdownerjobtest.cpp"
