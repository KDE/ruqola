/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelhistoryjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelhistoryjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ChannelHistoryJobTest)
using namespace RocketChatRestApi;
ChannelHistoryJobTest::ChannelHistoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelHistoryJobTest::shouldHaveDefaultValue()
{
    ChannelHistoryJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
    info.channelType = ChannelHistoryJob::ChannelType::Channel;
    job.setChannelHistoryInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.history?inclusive=false&unreads=false"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);

    info.channelType = ChannelHistoryJob::ChannelType::Direct;
    job.setChannelHistoryInfo(info);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/im.history?inclusive=false&unreads=false"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);

    info.channelType = ChannelHistoryJob::ChannelType::Groups;
    job.setChannelHistoryInfo(info);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.history?inclusive=false&unreads=false"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);

    info.roomId = "foo"_ba;
    info.channelType = ChannelHistoryJob::ChannelType::Groups;
    job.setChannelHistoryInfo(info);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.history?roomId=foo&inclusive=false&unreads=false"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChannelHistoryJobTest::shouldGenerateJson()
{
    //    ChannelHistoryJob job;
    //    const QString roomId = u"foo1"_s;
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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QVERIFY(!job.canStart());
    ChannelHistoryJob::ChannelHistoryInfo historyInfo;
    historyInfo.channelType = ChannelHistoryJob::ChannelType::Channel;

    job.setChannelHistoryInfo(historyInfo);
    QVERIFY(!job.canStart());
    historyInfo.roomId = "bla"_ba;
    job.setChannelHistoryInfo(historyInfo);
    QVERIFY(job.canStart());
}

#include "moc_channelhistoryjobtest.cpp"
