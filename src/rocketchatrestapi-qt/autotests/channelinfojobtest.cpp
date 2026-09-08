/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelinfojob.h"
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
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    info.identifier = u"foo"_s;
    job.setChannelGroupInfo(info);

    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.info?roomId=foo"_s));
}

void ChannelInfoJobTest::shouldNotStarting()
{
    ChannelInfoJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_channelinfojobtest.cpp"
