/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userscreatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include "users/userscreatejob.h"
#include <QTest>

QTEST_GUILESS_MAIN(UsersCreateJobTest)
using namespace RocketChatRestApi;
UsersCreateJobTest::UsersCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersCreateJobTest::shouldHaveDefaultValue()
{
    UsersCreateJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UsersCreateJobTest::shouldGenerateRequest()
{
    UsersCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.create"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UsersCreateJobTest::shouldGenerateJson()
{
    UsersCreateJob job;
    CreateUpdateUserInfo info;
    const QString password{u"ccc"_s};
    info.mPassword = password;
    const QString email{u"bla@kde.org"_s};
    info.mEmail = email;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(R"({"email":"%1","password":"%2","requirePasswordChange":false,"sendWelcomeEmail":false,"setRandomPassword":false,"verified":false})")
            .arg(email, password)
            .toLatin1());

    const QString nickame{u"blu"_s};
    info.mNickName = nickame;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"email":"%1","nickname":"%3","password":"%2","requirePasswordChange":false,"sendWelcomeEmail":false,"setRandomPassword":false,"verified":false})")
            .arg(email, password, nickame)
            .toLatin1());

    const QStringList roles{u"cd"_s, u"ssc"_s};
    info.mRoles = roles;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"email":"%1","nickname":"%3","password":"%2","requirePasswordChange":false,"roles":["cd","ssc"],"sendWelcomeEmail":false,"setRandomPassword":false,"verified":false})")
            .arg(email, password, nickame)
            .toLatin1());
    info.mRequirePasswordChange = true;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"email":"%1","nickname":"%3","password":"%2","requirePasswordChange":true,"roles":["cd","ssc"],"sendWelcomeEmail":false,"setRandomPassword":false,"verified":false})")
            .arg(email, password, nickame)
            .toLatin1());

    info.mSendWelcomeEmail = true;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"email":"%1","nickname":"%3","password":"%2","requirePasswordChange":true,"roles":["cd","ssc"],"sendWelcomeEmail":true,"setRandomPassword":false,"verified":false})")
            .arg(email, password, nickame)
            .toLatin1());

    info.mVerified = true;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"email":"%1","nickname":"%3","password":"%2","requirePasswordChange":true,"roles":["cd","ssc"],"sendWelcomeEmail":true,"setRandomPassword":false,"verified":true})")
            .arg(email, password, nickame)
            .toLatin1());

    info.mSetRandomPassword = false;
    info.mPassword = u"ccc"_s;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"email":"%1","nickname":"%3","password":"%2","requirePasswordChange":true,"roles":["cd","ssc"],"sendWelcomeEmail":true,"setRandomPassword":false,"verified":true})")
            .arg(email, password, nickame)
            .toLatin1());
}

void UsersCreateJobTest::shouldNotStarting()
{
    UsersCreateJob job;

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

    CreateUpdateUserInfo info;
    info.mPassword = u"ccc"_s;
    job.setCreateInfo(info);

    QVERIFY(!job.canStart());
    info.mEmail = u"ccc"_s;
    job.setCreateInfo(info);

    QVERIFY(!job.canStart());
    info.mName = u"777"_s;
    job.setCreateInfo(info);

    QVERIFY(job.canStart());
}

#include "moc_userscreatejobtest.cpp"
