/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelremovemoderatorjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelremovemoderatorjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ChannelRemoveModeratorJobTest)
using namespace RocketChatRestApi;
ChannelRemoveModeratorJobTest::ChannelRemoveModeratorJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelRemoveModeratorJobTest::shouldHaveDefaultValue()
{
    ChannelRemoveModeratorJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.removeUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelRemoveModeratorJobTest::shouldGenerateRequest()
{
    ChannelRemoveModeratorJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.removeModerator"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChannelRemoveModeratorJobTest::shouldGenerateJson()
{
    ChannelRemoveModeratorJob job;
    const QString roomId = u"foo1"_s;
    const QString removeUserId = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setRemoveUserId(removeUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(removeUserId, roomId).toLatin1());
}

#include "moc_channelremovemoderatorjobtest.cpp"
