/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersdisabletotpjobtest.h"

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "users/usersdisabletotpjob.h"
#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(UsersDisableTotpJobTest)
using namespace RocketChatRestApi;
UsersDisableTotpJobTest::UsersDisableTotpJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersDisableTotpJobTest::shouldHaveDefaultValue()
{
    UsersDisableTotpJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.code().isEmpty());
}

void UsersDisableTotpJobTest::shouldGenerateRequest()
{
    UsersDisableTotpJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.disableTotp"_s));
}

void UsersDisableTotpJobTest::shouldGenerateJson()
{
    UsersDisableTotpJob job;
    const QString code = u"123456"_s;
    job.setCode(code);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"code":"%1"})"_s.arg(code).toLatin1());
}

void UsersDisableTotpJobTest::shouldNotStarting()
{
    UsersDisableTotpJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);

    job.setCode(u"123456"_s);
    QVERIFY(job.canStart());
}

#include "moc_usersdisabletotpjobtest.cpp"
