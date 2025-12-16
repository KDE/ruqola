/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelclosejobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelclosejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChannelCloseJobTest)
using namespace RocketChatRestApi;
ChannelCloseJobTest::ChannelCloseJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelCloseJobTest::shouldHaveDefaultValue()
{
    ChannelCloseJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QCOMPARE(job.channelType(), ChannelCloseJob::ChannelType::Unknown);
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelCloseJobTest::shouldGenerateRequest()
{
    ChannelCloseJob job;
    job.setChannelType(ChannelCloseJob::ChannelType::Channel);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.close"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);

    job.setChannelType(ChannelCloseJob::ChannelType::Direct);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/im.close"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);

    job.setChannelType(ChannelCloseJob::ChannelType::Groups);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.close"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChannelCloseJobTest::shouldGenerateJson()
{
    ChannelCloseJob job;
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

void ChannelCloseJobTest::shouldNotStarting()
{
    ChannelCloseJob job;

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
    job.setChannelType(ChannelCloseJob::ChannelType::Channel);
    QVERIFY(job.canStart());
}

#include "moc_channelclosejobtest.cpp"
