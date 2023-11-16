/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setchanneltypejobtest.h"
#include "channels/setchanneltypejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SetChannelTypeJobTest)
using namespace RocketChatRestApi;
SetChannelTypeJobTest::SetChannelTypeJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetChannelTypeJobTest::shouldHaveDefaultValue()
{
    SetChannelTypeJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QCOMPARE(job.type(), SetChannelTypeJob::Unknown);
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SetChannelTypeJobTest::shouldGenerateRequest()
{
    SetChannelTypeJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setType")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SetChannelTypeJobTest::shouldGenerateJson()
{
    SetChannelTypeJob job;
    const QString channelname = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = channelname;
    job.setChannelGroupInfo(info);
    job.setType(SetChannelTypeJob::Private);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","type":"p"})").arg(channelname).toLatin1());

    job.setType(SetChannelTypeJob::Public);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","type":"c"})").arg(channelname).toLatin1());
}

void SetChannelTypeJobTest::shouldNotStarting()
{
    SetChannelTypeJob job;

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
    job.setType(SetChannelTypeJob::Private);
    QVERIFY(job.canStart());
}

#include "moc_setchanneltypejobtest.cpp"
