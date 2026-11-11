/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userstotpcodesremainingjobtest.h"

#include "restapimethod.h"
#include "users/userstotpcodesremainingjob.h"
#include <QNetworkAccessManager>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(UsersTotpCodesRemainingJobTest)
using namespace RocketChatRestApi;
UsersTotpCodesRemainingJobTest::UsersTotpCodesRemainingJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersTotpCodesRemainingJobTest::shouldHaveDefaultValue()
{
    UsersTotpCodesRemainingJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void UsersTotpCodesRemainingJobTest::shouldGenerateRequest()
{
    UsersTotpCodesRemainingJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.totpCodesRemaining"_s));
}

void UsersTotpCodesRemainingJobTest::shouldNotStarting()
{
    UsersTotpCodesRemainingJob job;

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

#include "moc_userstotpcodesremainingjobtest.cpp"
