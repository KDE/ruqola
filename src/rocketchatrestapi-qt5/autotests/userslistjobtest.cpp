/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "userslistjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/userslistjob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(UsersListJobTest)
using namespace RocketChatRestApi;
UsersListJobTest::UsersListJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersListJobTest::shouldHaveDefaultValue()
{
    UsersListJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(job.hasQueryParameterSupport());
}

void UsersListJobTest::shouldGenerateRequest()
{
    UsersListJob job;
    UsersListJob::UserInfo info;
    info.userIdentifier = QStringLiteral("foo1");
    info.userInfoType = UsersListJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QEXPECT_FAIL("", "Problem with argument", Continue);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.list?userId=foo1")));
}

void UsersListJobTest::shouldGenerateRequestUsername()
{
    UsersListJob job;
    UsersListJob::UserInfo info;
    info.userIdentifier = QStringLiteral("foo1");
    info.userInfoType = UsersListJob::UserInfoType::UserName;
    job.setUserInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QEXPECT_FAIL("", "Problem with argument", Continue);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.list?username=foo1")));
}
