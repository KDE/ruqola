/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetavatarjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"

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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"userId":"%1"})"_s.arg(info.userIdentifier).toLatin1());
}

void ResetAvatarJobTest::shouldNotStarting()
{
    ResetAvatarJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    UserBaseJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UserBaseJob::UserInfoType::UserName;
    job.setUserInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_resetavatarjobtest.cpp"
