/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "channelhistoryjobtest.h"
#include "channels/channelhistoryjob.h"
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
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasRoomIdentifier());
    const ChannelHistoryJob::ChannelHistoryInfo info = job.channelHistoryInfo();
    QCOMPARE(info.channelType, ChannelHistoryJob::ChannelType::Unknown);
    QCOMPARE(info.count, -1);
    QVERIFY(info.latestMessage.isEmpty());
    QVERIFY(info.oldestMessage.isEmpty());
    QCOMPARE(info.offset, 0);
    QVERIFY(!info.inclusive);

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
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.history")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));

    info.channelType = ChannelHistoryJob::Direct;
    job.setChannelHistoryInfo(info);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.history")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));

    info.channelType = ChannelHistoryJob::Groups;
    job.setChannelHistoryInfo(info);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.history")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelHistoryJobTest::shouldGenerateJson()
{
    ChannelHistoryJob job;
    const QString roomId = QStringLiteral("foo1");
    ChannelBaseJob::ChannelInfo info;
    info.channelInfoType = ChannelBaseJob::ChannelInfoType::RoomId;
    info.channelInfoIdentifier = roomId;
    job.setChannelInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
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
    ChannelBaseJob::ChannelInfo info;
    info.channelInfoType = ChannelBaseJob::ChannelInfoType::RoomId;
    info.channelInfoIdentifier = roomId;
    job.setChannelInfo(info);
    QVERIFY(!job.canStart());
    ChannelHistoryJob::ChannelHistoryInfo historyInfo;
    historyInfo.channelType = ChannelHistoryJob::Channel;

    job.setChannelHistoryInfo(historyInfo);
    QVERIFY(job.canStart());
}
