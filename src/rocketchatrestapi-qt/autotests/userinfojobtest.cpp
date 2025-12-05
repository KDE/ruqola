/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "users/userinfojob.h"
QTEST_GUILESS_MAIN(UserInfoJobTest)
using namespace RocketChatRestApi;
UserInfoJobTest::UserInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void UserInfoJobTest::shouldHaveDefaultValue()
{
    UserInfoJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UserInfoJobTest::shouldGenerateRequest()
{
    UserInfoJob job;
    UserInfoJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UserInfoJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.info?userId=foo1"_s));
}

void UserInfoJobTest::shouldGenerateRequestUsername()
{
    UserInfoJob job;
    UserInfoJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UserInfoJob::UserInfoType::UserName;
    job.setUserInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.info?username=foo1"_s));
}

#include "moc_userinfojobtest.cpp"
