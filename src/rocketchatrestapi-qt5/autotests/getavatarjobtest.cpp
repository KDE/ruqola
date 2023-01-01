/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getavatarjobtest.h"
#include "restapimethod.h"
#include "users/getavatarjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(GetAvatarJobTest)
using namespace RocketChatRestApi;
GetAvatarJobTest::GetAvatarJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetAvatarJobTest::shouldHaveDefaultValue()
{
    GetAvatarJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetAvatarJobTest::shouldGenerateRequest()
{
    GetAvatarJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    UserBaseJob::UserInfo info;
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    const QString avatarUserId = QStringLiteral("avat");
    info.userIdentifier = avatarUserId;
    job.setUserInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.getAvatar?userId=avat")));
}
