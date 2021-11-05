/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "userscreatejobtest.h"
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
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UsersCreateJobTest::shouldGenerateRequest()
{
    UsersCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UsersCreateJobTest::shouldGenerateJson()
{
    UsersCreateJob job;
    CreateUpdateUserInfo info;
    const QString password{QStringLiteral("ccc")};
    info.mPassword = password;
    const QString email{QStringLiteral("bla@kde.org")};
    info.mEmail = email;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(R"({"email":"%1","password":"%2","requirePasswordChange":false,"sendWelcomeEmail":false,"setRandomPassword":false,"verified":false})")
            .arg(email, password)
            .toLatin1());

    const QString nickame{QStringLiteral("blu")};
    info.mNickName = nickame;
    job.setCreateInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"email":"%1","nickname":"%3","password":"%2","requirePasswordChange":false,"sendWelcomeEmail":false,"setRandomPassword":false,"verified":false})")
            .arg(email, password, nickame)
            .toLatin1());

    const QStringList roles{QStringLiteral("cd"), QStringLiteral("ssc")};
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
}

void UsersCreateJobTest::shouldNotStarting()
{
    UsersCreateJob job;

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

    CreateUpdateUserInfo info;
    info.mUserId = QStringLiteral("userid");
    info.mPassword = QStringLiteral("ccc");
    job.setCreateInfo(info);

    QVERIFY(job.canStart());
}
