/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersupdateownbasicinfojobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/usersupdateownbasicinfojob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UsersUpdateOwnBasicInfoJobTest)
using namespace RocketChatRestApi;
UsersUpdateOwnBasicInfoJobTest::UsersUpdateOwnBasicInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersUpdateOwnBasicInfoJobTest::shouldHaveDefaultValue()
{
    UsersUpdateOwnBasicInfoJob job;
    QVERIFY(!job.updateOwnBasicInfo().isValid());
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UsersUpdateOwnBasicInfoJobTest::shouldGenerateRequest()
{
    UsersUpdateOwnBasicInfoJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.updateOwnBasicInfo")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UsersUpdateOwnBasicInfoJobTest::shouldGenerateJson()
{
    UsersUpdateOwnBasicInfoJob job;
    UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo info;
    const QString email = QStringLiteral("foo@kde.org");
    info.email = email;
    info.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Email;
    job.setUpdateOwnBasicInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"data":{"email":"%1"}})").arg(email).toLatin1());

    const QString username = QStringLiteral("username");
    info.userName = username;
    info.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::UserName;
    job.setUpdateOwnBasicInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"data":{"email":"%1","username":"%2"}})").arg(email, username).toLatin1());

    const QString nickname = QStringLiteral("nick");
    info.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::NickName;
    info.nickName = nickname;
    job.setUpdateOwnBasicInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"data":{"email":"%1","nickname":"%3","username":"%2"}})").arg(email, username, nickname).toLatin1());

    const QString statustext = QStringLiteral("tt");
    info.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::StatusText;
    info.statusText = statustext;
    job.setUpdateOwnBasicInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(R"({"data":{"email":"%1","nickname":"%3","statusText":"%4","username":"%2"}})").arg(email, username, nickname, statustext).toLatin1());
}

void UsersUpdateOwnBasicInfoJobTest::shouldNotStarting()
{
    UsersUpdateOwnBasicInfoJob job;

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
    UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo info;
    const QString email = QStringLiteral("foo@kde.org");
    info.email = email;
    info.type = UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Email;
    job.setUpdateOwnBasicInfo(info);

    // TODO
    QVERIFY(job.canStart());
}

#include "moc_usersupdateownbasicinfojobtest.cpp"
