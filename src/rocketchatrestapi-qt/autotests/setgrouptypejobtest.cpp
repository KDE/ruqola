/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setgrouptypejobtest.h"
#include "groups/setgrouptypejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SetGroupTypeJobTest)
using namespace RocketChatRestApi;
SetGroupTypeJobTest::SetGroupTypeJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetGroupTypeJobTest::shouldHaveDefaultValue()
{
    SetGroupTypeJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QCOMPARE(job.type(), SetGroupTypeJob::Unknown);
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SetGroupTypeJobTest::shouldGenerateRequest()
{
    SetGroupTypeJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setType")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SetGroupTypeJobTest::shouldGenerateJson()
{
    SetGroupTypeJob job;
    const QString channelname = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = channelname;
    job.setChannelGroupInfo(info);
    job.setType(SetGroupTypeJob::Private);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","type":"p"})").arg(channelname).toLatin1());

    job.setType(SetGroupTypeJob::Public);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","type":"c"})").arg(channelname).toLatin1());
}

void SetGroupTypeJobTest::shouldNotStarting()
{
    SetGroupTypeJob job;

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
    job.setType(SetGroupTypeJob::Private);
    QVERIFY(job.canStart());
}

#include "moc_setgrouptypejobtest.cpp"
