/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeladdmoderatorjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channeladdmoderatorjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ChannelAddModeratorJobTest)
using namespace RocketChatRestApi;
ChannelAddModeratorJobTest::ChannelAddModeratorJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelAddModeratorJobTest::shouldHaveDefaultValue()
{
    ChannelAddModeratorJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.addModeratorUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelAddModeratorJobTest::shouldGenerateRequest()
{
    ChannelAddModeratorJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.addModerator"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChannelAddModeratorJobTest::shouldGenerateJson()
{
    ChannelAddModeratorJob job;
    const QString roomId = u"foo1"_s;
    const QString addUsedId = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setAddModeratorUserId(addUsedId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(addUsedId, roomId).toLatin1());
}

#include "moc_channeladdmoderatorjobtest.cpp"
