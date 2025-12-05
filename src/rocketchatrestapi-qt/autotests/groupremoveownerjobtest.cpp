/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupremoveownerjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/groupremoveownerjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupRemoveOwnerJobTest)
using namespace RocketChatRestApi;
GroupRemoveOwnerJobTest::GroupRemoveOwnerJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupRemoveOwnerJobTest::shouldHaveDefaultValue()
{
    GroupRemoveOwnerJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.removeUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupRemoveOwnerJobTest::shouldGenerateRequest()
{
    GroupRemoveOwnerJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.removeOwner"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void GroupRemoveOwnerJobTest::shouldGenerateJson()
{
    GroupRemoveOwnerJob job;
    const QString roomId = u"foo1"_s;
    const QString removeUserId = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setRemoveUserId(removeUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(removeUserId, roomId).toLatin1());
}

#include "moc_groupremoveownerjobtest.cpp"
