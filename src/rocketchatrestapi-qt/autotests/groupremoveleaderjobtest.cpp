/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupremoveleaderjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/groupremoveleaderjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupRemoveLeaderJobTest)
using namespace RocketChatRestApi;
GroupRemoveLeaderJobTest::GroupRemoveLeaderJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupRemoveLeaderJobTest::shouldHaveDefaultValue()
{
    GroupRemoveLeaderJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.removeUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupRemoveLeaderJobTest::shouldGenerateRequest()
{
    GroupRemoveLeaderJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.removeLeader"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void GroupRemoveLeaderJobTest::shouldGenerateJson()
{
    GroupRemoveLeaderJob job;
    const QString roomId = u"foo1"_s;
    const QString removeUserId = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job.setChannelGroupInfo(info);
    job.setRemoveUserId(removeUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(removeUserId, roomId).toLatin1());
}

#include "moc_groupremoveleaderjobtest.cpp"
