/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersupdatejobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/usersupdatejob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UsersUpdateJobTest)
using namespace RocketChatRestApi;
UsersUpdateJobTest::UsersUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersUpdateJobTest::shouldHaveDefaultValue()
{
    UsersUpdateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
}

void UsersUpdateJobTest::shouldGenerateRequest()
{
    UsersUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UsersUpdateJobTest::shouldGenerateJson()
{
    UsersUpdateJob job;
    UpdateUserInfo info;
    info.mTypeInfo = CreateUpdateUserInfo::Update;

    const QString mUserId = QStringLiteral("foo");

    const QString mEmail = QStringLiteral("bla@kde.org");
    const QString mName = QStringLiteral("name_bla");
    const QString mUserName = QStringLiteral("username_bla");
    const QString mPassword = QStringLiteral("password_bla");

    job.setUpdateInfo(info);
    info.mUserId = mUserId;
    job.setUpdateInfo(info);

    info.mEmail = mEmail;
    info.mName = mName;
    info.mUserName = mUserName;
    info.mPassword = mPassword;
    job.setUpdateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"data":{"email":"%1","name":"%2","password":"%4","requirePasswordChange":false,"sendWelcomeEmail":false,"setRandomPassword":false,"username":"%3","verified":false},"userId":"%5"})")
            .arg(mEmail, mName, mUserName, mPassword, mUserId)
            .toLatin1());
}

void UsersUpdateJobTest::shouldNotStarting()
{
    UsersUpdateJob job;

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

    UpdateUserInfo info;
    info.mUserId = QStringLiteral("userid");
    job.setUpdateInfo(info);
    QVERIFY(!job.canStart());

    info.mName = QStringLiteral("mname");
    job.setUpdateInfo(info);
    QVERIFY(!job.canStart());

    info.mEmail = QStringLiteral("emil");
    job.setUpdateInfo(info);
    QVERIFY(!job.canStart());

    job.setAuthCode(QStringLiteral("bla"));
    QVERIFY(!job.canStart());

    job.setAuthMethod(QStringLiteral("method"));
    QVERIFY(job.canStart());
}

#include "moc_usersupdatejobtest.cpp"
