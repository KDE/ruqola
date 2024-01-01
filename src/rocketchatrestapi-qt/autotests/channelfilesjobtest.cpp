/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelfilesjobtest.h"
#include "channels/channelfilesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(ChannelFilesJobTest)
using namespace RocketChatRestApi;
ChannelFilesJobTest::ChannelFilesJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelFilesJobTest::shouldHaveDefaultValue()
{
    ChannelFilesJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QCOMPARE(job.channelType(), ChannelFilesJob::ChannelType::Unknown);
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void ChannelFilesJobTest::shouldGenerateRequest()
{
    ChannelFilesJob job;
    job.setChannelType(ChannelFilesJob::Channel);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = QStringLiteral("foo");
    job.setChannelGroupInfo(info);

    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.files?roomId=foo")));

    job.setChannelType(ChannelFilesJob::Direct);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.files?roomId=foo")));

    job.setChannelType(ChannelFilesJob::Groups);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.files?roomId=foo")));
}

void ChannelFilesJobTest::shouldNotStarting()
{
    ChannelFilesJob job;

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
    job.setChannelType(ChannelFilesJob::ChannelType::Channel);
    QVERIFY(job.canStart());
}

#include "moc_channelfilesjobtest.cpp"
