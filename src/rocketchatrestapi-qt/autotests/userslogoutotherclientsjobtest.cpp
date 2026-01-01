/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslogoutotherclientsjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
}

void UsersLogoutOtherClientsJobTest::shouldGenerateRequest()
{
    UsersLogoutOtherClientsJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.logoutOtherClients"_s));
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
    QVERIFY(job.canStart());
}

#include "moc_userslogoutotherclientsjobtest.cpp"
