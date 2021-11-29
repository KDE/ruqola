/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    QVERIFY(job.roomId().isEmpty());
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
    job.setRoomId(roomId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.roles?roomId=avat")));
}
