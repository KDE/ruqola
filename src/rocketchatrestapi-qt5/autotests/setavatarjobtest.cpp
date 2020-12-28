/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "setavatarjobtest.h"
#include "users/setavatarjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SetAvatarJobTest)
using namespace RocketChatRestApi;
SetAvatarJobTest::SetAvatarJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetAvatarJobTest::shouldHaveDefaultValue()
{
    SetAvatarJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.avatarInfo().isValid());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetAvatarJobTest::shouldGenerateRequest()
{
    SetAvatarJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setAvatar")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SetAvatarJobTest::shouldGenerateJson()
{
    SetAvatarJob job;

    const QString avatarurl = QStringLiteral("foo1");

    SetAvatarJob::SetAvatarInfo avatarInfo;
    avatarInfo.mAvatarUrl = avatarurl;
    job.setAvatarInfo(avatarInfo);

    UserBaseJob::UserInfo info;
    info.userIdentifier = QStringLiteral("foo");
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    job.setUserInfo(info);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"avatarUrl":"%1","userId":"%2"})")
             .arg(avatarurl, info.userIdentifier).toLatin1());
}

void SetAvatarJobTest::shouldNotStarting()
{
    SetAvatarJob job;

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

    const QString avatarurl = QStringLiteral("foo1");
    SetAvatarJob::SetAvatarInfo avatarInfo;
    avatarInfo.mAvatarUrl = avatarurl;
    job.setAvatarInfo(avatarInfo);

    QVERIFY(!job.canStart());
    UserBaseJob::UserInfo info;
    info.userIdentifier = QStringLiteral("foo");
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QVERIFY(job.canStart());
}
