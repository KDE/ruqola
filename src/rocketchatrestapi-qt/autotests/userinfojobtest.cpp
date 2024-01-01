/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userinfojobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/userinfojob.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(UserInfoJobTest)
using namespace RocketChatRestApi;
UserInfoJobTest::UserInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void UserInfoJobTest::shouldHaveDefaultValue()
{
    UserInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UserInfoJobTest::shouldGenerateRequest()
{
    UserInfoJob job;
    UserInfoJob::UserInfo info;
    info.userIdentifier = QStringLiteral("foo1");
    info.userInfoType = UserInfoJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.info?userId=foo1")));
}

void UserInfoJobTest::shouldGenerateRequestUsername()
{
    UserInfoJob job;
    UserInfoJob::UserInfo info;
    info.userIdentifier = QStringLiteral("foo1");
    info.userInfoType = UserInfoJob::UserInfoType::UserName;
    job.setUserInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.info?username=foo1")));
}

#include "moc_userinfojobtest.cpp"
