/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/registeruserjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(RegisterUserJobTest)
using namespace RocketChatRestApi;
RegisterUserJobTest::RegisterUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void RegisterUserJobTest::shouldHaveDefaultValue()
{
    RegisterUserJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.registerUserInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RegisterUserJobTest::shouldGenerateRequest()
{
    RegisterUserJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.register")));
}

void RegisterUserJobTest::shouldGenerateJson()
{
    RegisterUserJob job;
    RegisterUserJob::RegisterUserInfo info;
    const QString password = QStringLiteral("foo1");
    const QString email = QStringLiteral("bla@bli.com");
    const QString name = QStringLiteral("name");
    const QString username = QStringLiteral("username");
    info.email = email;
    info.username = username;
    info.name = name;
    info.password = password;
    job.setRegisterUserInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"email":"%1","name":"%2","pass":"%3","username":"%4"})").arg(email, name, password, username).toLatin1());
}

void RegisterUserJobTest::shouldNotStarting()
{
    RegisterUserJob job;

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
    RegisterUserJob::RegisterUserInfo info;
    const QString password = QStringLiteral("foo1");
    const QString email = QStringLiteral("bla@bli.com");
    const QString name = QStringLiteral("name");
    const QString username = QStringLiteral("username");
    info.email = email;
    job.setRegisterUserInfo(info);
    QVERIFY(!job.canStart());
    info.username = username;
    job.setRegisterUserInfo(info);
    QVERIFY(!job.canStart());
    info.name = name;
    job.setRegisterUserInfo(info);
    QVERIFY(!job.canStart());
    info.password = password;
    job.setRegisterUserInfo(info);
    QVERIFY(job.canStart());
}
