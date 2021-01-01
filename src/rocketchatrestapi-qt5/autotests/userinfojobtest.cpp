/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "userinfojobtest.h"
#include "users/userinfojob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
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
