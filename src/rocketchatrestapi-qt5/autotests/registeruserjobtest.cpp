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

#include "registeruserjobtest.h"
#include "users/registeruserjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"email":"%1","name":"%2","pass":"%3","username":"%4"})")
             .arg(email, name, password, username).toLatin1());
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
