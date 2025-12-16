/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersupdatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "users/usersupdatejob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(UsersUpdateJobTest)
using namespace RocketChatRestApi;
UsersUpdateJobTest::UsersUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersUpdateJobTest::shouldHaveDefaultValue()
{
    UsersUpdateJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
}

void UsersUpdateJobTest::shouldGenerateRequest()
{
    UsersUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.update"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UsersUpdateJobTest::shouldGenerateJson()
{
    UsersUpdateJob job;
    UpdateUserInfo info;
    info.mTypeInfo = CreateUpdateUserInfo::Update;

    const QString mUserId = u"foo"_s;

    const QString mEmail = u"bla@kde.org"_s;
    const QString mName = u"name_bla"_s;
    const QString mUserName = u"username_bla"_s;
    const QString mPassword = u"password_bla"_s;

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

    UpdateUserInfo info;
    info.mUserId = u"userid"_s;
    job.setUpdateInfo(info);
    QVERIFY(!job.canStart());

    info.mName = u"mname"_s;
    job.setUpdateInfo(info);
    QVERIFY(!job.canStart());

    info.mEmail = u"emil"_s;
    job.setUpdateInfo(info);
    QVERIFY(!job.canStart());

    job.setAuthCode(u"bla"_s);
    QVERIFY(!job.canStart());

    job.setAuthMethod(u"method"_s);
    QVERIFY(job.canStart());
}

#include "moc_usersupdatejobtest.cpp"
