/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupaddownerjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/groupaddownerjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GroupAddOwnerJobTest)
using namespace RocketChatRestApi;
GroupAddOwnerJobTest::GroupAddOwnerJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupAddOwnerJobTest::shouldHaveDefaultValue()
{
    GroupAddOwnerJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.addownerUserId().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupAddOwnerJobTest::shouldGenerateRequest()
{
    GroupAddOwnerJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.addOwner"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void GroupAddOwnerJobTest::shouldGenerateJson()
{
    GroupAddOwnerJob job;
    const QString roomId = u"foo1"_s;
    const QString addUsedId = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job.setChannelGroupInfo(info);
    job.setAddownerUserId(addUsedId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%2","userId":"%1"})").arg(addUsedId, roomId).toLatin1());
}

#include "moc_groupaddownerjobtest.cpp"
