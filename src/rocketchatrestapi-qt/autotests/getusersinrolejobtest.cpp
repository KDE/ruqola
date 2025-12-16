/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersinrolejobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString roleId{u"bla"_s};
    job.setRoleId(roleId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/roles.getUsersInRole?role=%1"_s.arg(roleId)));
}

void GetUsersInRoleJobTest::shouldNotStarting()
{
    GetUsersInRoleJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);

    QVERIFY(!job.canStart());
    job.setRoleId(u"ss"_s);
    QVERIFY(job.canStart());
}

#include "moc_getusersinrolejobtest.cpp"
