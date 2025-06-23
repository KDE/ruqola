/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsmoderatorsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelsmoderatorsjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(ChannelsModeratorsJobTest)
using namespace RocketChatRestApi;
ChannelsModeratorsJobTest::ChannelsModeratorsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelsModeratorsJobTest::shouldHaveDefaultValue()
{
    ChannelsModeratorsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelsModeratorsJobTest::shouldGenerateRequest()
{
    ChannelsModeratorsJob job;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = u"foo"_s;
    job.setChannelGroupInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.moderators?roomId=foo"_s));
}

void ChannelsModeratorsJobTest::shouldNotStarting()
{
    ChannelsModeratorsJob job;

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
    QVERIFY(job.canStart());
}

#include "moc_channelsmoderatorsjobtest.cpp"
