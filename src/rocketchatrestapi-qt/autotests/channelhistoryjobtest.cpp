/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelhistoryjobtest.h"
#include "channels/channelhistoryjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChannelHistoryJobTest)
using namespace RocketChatRestApi;
ChannelHistoryJobTest::ChannelHistoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelHistoryJobTest::shouldHaveDefaultValue()
{
    ChannelHistoryJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    const ChannelHistoryJob::ChannelHistoryInfo info = job.channelHistoryInfo();
    QCOMPARE(info.channelType, ChannelHistoryJob::ChannelType::Unknown);
    QCOMPARE(info.count, -1);
    QVERIFY(info.latestMessage.isEmpty());
    QVERIFY(info.oldestMessage.isEmpty());
    QCOMPARE(info.offset, -1);
    QVERIFY(!info.inclusive);
    QVERIFY(!info.unreads);

    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelHistoryJobTest::shouldGenerateRequest()
{
    ChannelHistoryJob job;
    ChannelHistoryJob::ChannelHistoryInfo info;
    info.channelType = ChannelHistoryJob::Channel;
    job.setChannelHistoryInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.history?roomId&inclusive=false&unreads=false")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));

    info.channelType = ChannelHistoryJob::Direct;
    job.setChannelHistoryInfo(info);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.history?roomId&inclusive=false&unreads=false")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));

    info.channelType = ChannelHistoryJob::Groups;
    job.setChannelHistoryInfo(info);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.history?roomId&inclusive=false&unreads=false")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelHistoryJobTest::shouldGenerateJson()
{
    //    ChannelHistoryJob job;
    //    const QString roomId = QStringLiteral("foo1");
    //    ChannelBaseJob::ChannelInfo info;
    //    info.channelGroupInfoType = ChannelBaseJob::ChannelInfoType::RoomId;
    //    info.identifier = roomId;
    //    job.setChannelGroupInfo(info);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

void ChannelHistoryJobTest::shouldNotStarting()
{
    ChannelHistoryJob job;

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
    QVERIFY(!job.canStart());
    ChannelHistoryJob::ChannelHistoryInfo historyInfo;
    historyInfo.channelType = ChannelHistoryJob::Channel;

    job.setChannelHistoryInfo(historyInfo);
    QVERIFY(!job.canStart());
    historyInfo.roomId = QByteArrayLiteral("bla");
    job.setChannelHistoryInfo(historyInfo);
    QVERIFY(job.canStart());
}

#include "moc_channelhistoryjobtest.cpp"
