/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "privateinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "settings/privateinfojob.h"
#include <QTest>

QTEST_GUILESS_MAIN(PrivateInfoJobTest)
using namespace RocketChatRestApi;
PrivateInfoJobTest::PrivateInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void PrivateInfoJobTest::shouldHaveDefaultValue()
{
    PrivateInfoJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void PrivateInfoJobTest::shouldGenerateRequest()
{
    PrivateInfoJob job;
    const QString authToken = u"foo"_s;
    const QString userId = u"user"_s;
    job.setUserId(userId);
    job.setAuthToken(authToken);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/settings"_s));
    QCOMPARE(request.rawHeader("X-Auth-Token"_ba), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader("X-User-Id"_ba), userId.toLocal8Bit());
}

#include "moc_privateinfojobtest.cpp"
