/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setchanneltypejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/setchanneltypejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SetChannelTypeJobTest)
using namespace RocketChatRestApi;
SetChannelTypeJobTest::SetChannelTypeJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetChannelTypeJobTest::shouldHaveDefaultValue()
{
    SetChannelTypeJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QCOMPARE(job.type(), SetChannelTypeJob::GroupType::Unknown);
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SetChannelTypeJobTest::shouldGenerateRequest()
{
    SetChannelTypeJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.setType"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetChannelTypeJobTest::shouldGenerateJson()
{
    SetChannelTypeJob job;
    const QString channelname = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = channelname;
    job.setChannelGroupInfo(info);
    job.setType(SetChannelTypeJob::GroupType::Private);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","type":"p"})").arg(channelname).toLatin1());

    job.setType(SetChannelTypeJob::GroupType::Public);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","type":"c"})").arg(channelname).toLatin1());
}

void SetChannelTypeJobTest::shouldNotStarting()
{
    SetChannelTypeJob job;

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
    job.setType(SetChannelTypeJob::GroupType::Private);
    QVERIFY(job.canStart());
}

#include "moc_setchanneltypejobtest.cpp"
