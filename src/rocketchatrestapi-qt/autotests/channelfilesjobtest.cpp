/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelfilesjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelfilesjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(ChannelFilesJobTest)
using namespace RocketChatRestApi;
ChannelFilesJobTest::ChannelFilesJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelFilesJobTest::shouldHaveDefaultValue()
{
    ChannelFilesJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QCOMPARE(job.channelType(), ChannelFilesJob::ChannelType::Unknown);
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void ChannelFilesJobTest::shouldGenerateRequest()
{
    ChannelFilesJob job;
    job.setChannelType(ChannelFilesJob::ChannelType::Channel);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = u"foo"_s;
    job.setChannelGroupInfo(info);

    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.files?roomId=foo"_s));

    job.setChannelType(ChannelFilesJob::ChannelType::Direct);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/im.files?roomId=foo"_s));

    job.setChannelType(ChannelFilesJob::ChannelType::Groups);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.files?roomId=foo"_s));
}

void ChannelFilesJobTest::shouldNotStarting()
{
    ChannelFilesJob job;

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
    job.setChannelType(ChannelFilesJob::ChannelType::Channel);
    QVERIFY(job.canStart());
}

#include "moc_channelfilesjobtest.cpp"
