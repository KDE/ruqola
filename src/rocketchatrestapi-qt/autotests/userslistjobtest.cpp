/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslistjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "users/userslistjob.h"
#include <QTest>

QTEST_GUILESS_MAIN(UsersListJobTest)
using namespace RocketChatRestApi;
UsersListJobTest::UsersListJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersListJobTest::shouldHaveDefaultValue()
{
    UsersListJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void UsersListJobTest::shouldGenerateRequest()
{
    UsersListJob job;
    UsersListJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UsersListJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QEXPECT_FAIL("", "Problem with argument", Continue);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.list?userId=foo1"_s));
}

void UsersListJobTest::shouldGenerateRequestUsername()
{
    UsersListJob job;
    UsersListJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UsersListJob::UserInfoType::UserName;
    job.setUserInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QEXPECT_FAIL("", "Problem with argument", Continue);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.list?username=foo1"_s));
}

#include "moc_userslistjobtest.cpp"
