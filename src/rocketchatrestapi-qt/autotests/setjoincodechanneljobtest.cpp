/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setjoincodechanneljobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/setjoincodechanneljob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SetJoinCodeChannelJobTest)
using namespace RocketChatRestApi;
SetJoinCodeChannelJobTest::SetJoinCodeChannelJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetJoinCodeChannelJobTest::shouldHaveDefaultValue()
{
    SetJoinCodeChannelJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetJoinCodeChannelJobTest::shouldGenerateRequest()
{
    SetJoinCodeChannelJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.setJoinCode"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetJoinCodeChannelJobTest::shouldGenerateJson()
{
    SetJoinCodeChannelJob job;
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    const QString joinCode = u"bli"_s;
    job.setJoinCode(joinCode);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"joinCode":"%2","roomId":"%1"})").arg(roomId, joinCode).toLatin1());
}

void SetJoinCodeChannelJobTest::shouldNotStarting()
{
    SetJoinCodeChannelJob job;

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

    const QString joinCode = u"fd1"_s;
    job.setJoinCode(joinCode);
    QVERIFY(job.canStart());
}

#include "moc_setjoincodechanneljobtest.cpp"
