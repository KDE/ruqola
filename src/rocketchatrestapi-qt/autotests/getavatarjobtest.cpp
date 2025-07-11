/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getavatarjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    UserBaseJob::UserInfo info;
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    const QString avatarUserId = u"avat"_s;
    info.userIdentifier = avatarUserId;
    job.setUserInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.getAvatar?userId=avat"_s));
}

#include "moc_getavatarjobtest.cpp"
