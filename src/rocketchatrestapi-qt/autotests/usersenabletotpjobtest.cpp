/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersenabletotpjobtest.h"

#include "restapimethod.h"
#include "users/usersenabletotpjob.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(UsersEnableTotpJobTest)
using namespace RocketChatRestApi;
UsersEnableTotpJobTest::UsersEnableTotpJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersEnableTotpJobTest::shouldHaveDefaultValue()
{
    UsersEnableTotpJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void UsersEnableTotpJobTest::shouldGenerateRequest()
{
    UsersEnableTotpJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.enableTotp"_s));
}

void UsersEnableTotpJobTest::shouldGenerateJson()
{
    UsersEnableTotpJob job;
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), "{}"_ba);
}

void UsersEnableTotpJobTest::shouldNotStarting()
{
    UsersEnableTotpJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager networkAccessManager;
    job.setNetworkAccessManager(&networkAccessManager);
    QVERIFY(!job.canStart());
    job.setAuthToken(u"foo"_s);
    QVERIFY(!job.canStart());
    job.setUserId(u"foo"_s);
    QVERIFY(job.canStart());
}

#include "moc_usersenabletotpjobtest.cpp"
