/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersinrolejobtest.h"
#include "restapimethod.h"
#include "role/getusersinrolejob.h"
#include <QTest>
QTEST_GUILESS_MAIN(GetUsersInRoleJobTest)
using namespace RocketChatRestApi;
GetUsersInRoleJobTest::GetUsersInRoleJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetUsersInRoleJobTest::shouldHaveDefaultValue()
{
    GetUsersInRoleJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.roleId().isEmpty());
}

void GetUsersInRoleJobTest::shouldGenerateRequest()
{
    GetUsersInRoleJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roleId{QStringLiteral("bla")};
    job.setRoleId(roleId);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.getUsersInRole?role=%1").arg(roleId)));
}

void GetUsersInRoleJobTest::shouldNotStarting()
{
    GetUsersInRoleJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);

    QVERIFY(!job.canStart());
    job.setRoleId(QStringLiteral("ss"));
    QVERIFY(job.canStart());
}
