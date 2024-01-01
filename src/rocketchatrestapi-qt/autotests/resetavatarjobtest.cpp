/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetavatarjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/resetavatarjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ResetAvatarJobTest)
using namespace RocketChatRestApi;
ResetAvatarJobTest::ResetAvatarJobTest(QObject *parent)
    : QObject(parent)
{
}

void ResetAvatarJobTest::shouldHaveDefaultValue()
{
    ResetAvatarJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ResetAvatarJobTest::shouldGenerateRequest()
{
    ResetAvatarJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetAvatar")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ResetAvatarJobTest::shouldGenerateJson()
{
    ResetAvatarJob job;
    UserBaseJob::UserInfo info;
    info.userIdentifier = QStringLiteral("foo1");
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"userId":"%1"})").arg(info.userIdentifier).toLatin1());
}

void ResetAvatarJobTest::shouldNotStarting()
{
    ResetAvatarJob job;

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
    UserBaseJob::UserInfo info;
    info.userIdentifier = QStringLiteral("foo1");
    info.userInfoType = UserBaseJob::UserInfoType::UserName;
    job.setUserInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_resetavatarjobtest.cpp"
