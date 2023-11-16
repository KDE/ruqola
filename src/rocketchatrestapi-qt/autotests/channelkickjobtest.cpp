/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelkickjobtest.h"
#include "channels/channelkickjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChannelKickJobTest)
using namespace RocketChatRestApi;
ChannelKickJobTest::ChannelKickJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelKickJobTest::shouldHaveDefaultValue()
{
    ChannelKickJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(job.kickUserId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelKickJobTest::shouldGenerateRequest()
{
    ChannelKickJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.kick")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelKickJobTest::shouldGenerateJson()
{
    ChannelKickJob job;
    const QString channelname = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = channelname;
    job.setChannelGroupInfo(info);

    const QString userkick = QStringLiteral("bla");
    job.setKickUserId(userkick);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","userId":"%2"})").arg(channelname, userkick).toLatin1());
}

void ChannelKickJobTest::shouldNotStarting()
{
    ChannelKickJob job;

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
    const QString kickuser = QStringLiteral("bla");
    job.setKickUserId(kickuser);
    QVERIFY(job.canStart());
}

#include "moc_channelkickjobtest.cpp"
