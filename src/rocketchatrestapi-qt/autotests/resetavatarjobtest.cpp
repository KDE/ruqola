/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetavatarjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include "users/resetavatarjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ResetAvatarJobTest)
using namespace RocketChatRestApi;
ResetAvatarJobTest::ResetAvatarJobTest(QObject *parent)
    : QObject(parent)
{
}

void ResetAvatarJobTest::shouldHaveDefaultValue()
{
    ResetAvatarJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ResetAvatarJobTest::shouldGenerateRequest()
{
    ResetAvatarJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.resetAvatar"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ResetAvatarJobTest::shouldGenerateJson()
{
    ResetAvatarJob job;
    UserBaseJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"userId":"%1"})").arg(info.userIdentifier).toLatin1());
}

void ResetAvatarJobTest::shouldNotStarting()
{
    ResetAvatarJob job;

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
    UserBaseJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UserBaseJob::UserInfoType::UserName;
    job.setUserInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_resetavatarjobtest.cpp"
