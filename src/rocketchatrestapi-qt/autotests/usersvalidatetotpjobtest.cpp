/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersvalidatetotpjobtest.h"

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "users/usersvalidatetotpjob.h"
#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(UsersValidateTotpJobTest)
using namespace RocketChatRestApi;
UsersValidateTotpJobTest::UsersValidateTotpJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersValidateTotpJobTest::shouldHaveDefaultValue()
{
    UsersValidateTotpJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.code().isEmpty());
}

void UsersValidateTotpJobTest::shouldGenerateRequest()
{
    UsersValidateTotpJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.validateTotp"_s));
}

void UsersValidateTotpJobTest::shouldGenerateJson()
{
    UsersValidateTotpJob job;
    const QString code = u"123456"_s;
    job.setCode(code);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"code":"%1"})"_s.arg(code).toLatin1());
}

void UsersValidateTotpJobTest::shouldNotStarting()
{
    UsersValidateTotpJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);

    job.setCode(u"123456"_s);
    QVERIFY(job.canStart());
}

#include "moc_usersvalidatetotpjobtest.cpp"
