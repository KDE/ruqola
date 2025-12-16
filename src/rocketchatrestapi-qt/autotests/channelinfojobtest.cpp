/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfojobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelinfojob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

QTEST_GUILESS_MAIN(ChannelInfoJobTest)
using namespace RocketChatRestApi;
ChannelInfoJobTest::ChannelInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoJobTest::shouldHaveDefaultValue()
{
    ChannelInfoJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelInfoJobTest::shouldGenerateRequest()
{
    ChannelInfoJob job;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = u"foo"_s;
    job.setChannelGroupInfo(info);

    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.info?roomId=foo"_s));
}

void ChannelInfoJobTest::shouldNotStarting()
{
    ChannelInfoJob job;

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

#include "moc_channelinfojobtest.cpp"
