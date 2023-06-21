/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getgrouprolesjobtest.h"
#include "groups/getgrouprolesjob.h"
#include "restapimethod.h"
#include <QTest>
QTEST_GUILESS_MAIN(GetGroupRolesJobTest)
using namespace RocketChatRestApi;
GetGroupRolesJobTest::GetGroupRolesJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetGroupRolesJobTest::shouldHaveDefaultValue()
{
    GetGroupRolesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetGroupRolesJobTest::shouldGenerateRequest()
{
    GetGroupRolesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("avat");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.roles?roomId=avat")));
}

#include "moc_getgrouprolesjobtest.cpp"
