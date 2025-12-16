/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeldeletejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channeldeletejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChannelDeleteJobTest)
using namespace RocketChatRestApi;
ChannelDeleteJobTest::ChannelDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelDeleteJobTest::shouldHaveDefaultValue()
{
    ChannelDeleteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelDeleteJobTest::shouldGenerateRequest()
{
    ChannelDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChannelDeleteJobTest::shouldGenerateUserIdJson()
{
    ChannelDeleteJob job;
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

void ChannelDeleteJobTest::shouldGenerateUserNameJson()
{
    ChannelDeleteJob job;
    const QString roomId = u"foo1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

#include "moc_channeldeletejobtest.cpp"
