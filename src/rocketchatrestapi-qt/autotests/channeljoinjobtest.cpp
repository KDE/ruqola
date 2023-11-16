/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeljoinjobtest.h"
#include "channels/channeljoinjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChannelJoinJobTest)
using namespace RocketChatRestApi;
ChannelJoinJobTest::ChannelJoinJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelJoinJobTest::shouldHaveDefaultValue()
{
    ChannelJoinJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelJoinJobTest::shouldGenerateRequest()
{
    ChannelJoinJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.join")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelJoinJobTest::shouldGenerateJson()
{
    ChannelJoinJob job;
    const QString roomId = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    const QString joinCode = QStringLiteral("bli");
    job.setJoinCode(joinCode);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"joinCode":"%2","roomId":"%1"})").arg(roomId, joinCode).toLatin1());
}

void ChannelJoinJobTest::shouldNotStarting()
{
    ChannelJoinJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString roomId = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QVERIFY(job.canStart());

    // Join code is optional
    const QString joinCode = QStringLiteral("fd1");
    job.setJoinCode(joinCode);
    QVERIFY(job.canStart());
}

#include "moc_channeljoinjobtest.cpp"
