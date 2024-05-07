/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslogoutotherclientsjobtest.h"
#include "restapimethod.h"
#include "users/userslogoutotherclientsjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(UsersLogoutOtherClientsJobTest)
using namespace RocketChatRestApi;
UsersLogoutOtherClientsJobTest::UsersLogoutOtherClientsJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersLogoutOtherClientsJobTest::shouldHaveDefaultValue()
{
    UsersLogoutOtherClientsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
}

void UsersLogoutOtherClientsJobTest::shouldGenerateRequest()
{
    UsersLogoutOtherClientsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.logoutOtherClients")));
}

void UsersLogoutOtherClientsJobTest::shouldGenerateJson()
{
    UsersLogoutOtherClientsJob job;
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({})").toLatin1());
}

void UsersLogoutOtherClientsJobTest::shouldNotStarting()
{
    UsersLogoutOtherClientsJob job;

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
}

#include "moc_userslogoutotherclientsjobtest.cpp"
