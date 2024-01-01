/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletedmjobtest.h"
#include "directmessage/deletedmjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(DeleteDmJobTest)
using namespace RocketChatRestApi;
DeleteDmJobTest::DeleteDmJobTest(QObject *parent)
    : QObject(parent)
{
}

void DeleteDmJobTest::shouldHaveDefaultValue()
{
    DeleteDmJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void DeleteDmJobTest::shouldGenerateRequest()
{
    DeleteDmJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void DeleteDmJobTest::shouldGenerateUserIdJson()
{
    DeleteDmJob job;
    const QString roomId = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

void DeleteDmJobTest::shouldGenerateUserNameJson()
{
    DeleteDmJob job;
    const QString roomId = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

#include "moc_deletedmjobtest.cpp"
