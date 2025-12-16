/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelgetcountersjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelgetcountersjob.h"

#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(ChannelGetCountersJobTest)
using namespace RocketChatRestApi;
ChannelGetCountersJobTest::ChannelGetCountersJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelGetCountersJobTest::shouldHaveDefaultValue()
{
    ChannelGetCountersJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelGetCountersJobTest::shouldGenerateRequest()
{
    ChannelGetCountersJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = u"avat"_s;
    job.setChannelGroupInfo(info);
    QNetworkRequest request = job.request();
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.counters?roomId=avat"_s));
}

void ChannelGetCountersJobTest::shouldNotStarting()
{
    ChannelGetCountersJob job;

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
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = u"avat"_s;
    job.setChannelGroupInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_channelgetcountersjobtest.cpp"
