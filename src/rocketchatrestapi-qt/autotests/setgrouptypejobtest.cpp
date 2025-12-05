/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setgrouptypejobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.setType"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetGroupTypeJobTest::shouldGenerateJson()
{
    SetGroupTypeJob job;
    const QString channelname = u"foo1"_s;
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
    job.setType(SetGroupTypeJob::Private);
    QVERIFY(job.canStart());
}

#include "moc_setgrouptypejobtest.cpp"
