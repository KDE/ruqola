/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getchannelrolesjobtest.h"
#include "channels/getchannelrolesjob.h"
#include "restapimethod.h"
#include <QTest>
QTEST_GUILESS_MAIN(GetChannelRolesJobTest)
using namespace RocketChatRestApi;
GetChannelRolesJobTest::GetChannelRolesJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetChannelRolesJobTest::shouldHaveDefaultValue()
{
    GetChannelRolesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetChannelRolesJobTest::shouldGenerateRequest()
{
    GetChannelRolesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("avat");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.roles?roomId=avat")));
}

#include "moc_getchannelrolesjobtest.cpp"
