/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsnamejobtest.h"
#include "groups/changegroupsnamejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsNameJobTest)
using namespace RocketChatRestApi;
ChangeGroupsNameJobTest::ChangeGroupsNameJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsNameJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsNameJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.name().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsNameJobTest::shouldGenerateRequest()
{
    ChangeGroupsNameJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.rename")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeGroupsNameJobTest::shouldGenerateJson()
{
    ChangeGroupsNameJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString name = QStringLiteral("topic1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setName(name);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1","roomId":"%2"})").arg(name, roomId).toLatin1());
}
