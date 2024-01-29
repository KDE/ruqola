/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "publicsettingsjobtest.h"
#include "restapimethod.h"
#include "settings/publicsettingsjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(PublicSettingsJobTest)
using namespace RocketChatRestApi;
PublicSettingsJobTest::PublicSettingsJobTest(QObject *parent)
    : QObject(parent)
{
}

void PublicSettingsJobTest::shouldHaveDefaultValue()
{
    PublicSettingsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void PublicSettingsJobTest::shouldGenerateRequest()
{
    PublicSettingsJob job;
    const QString authToken = QStringLiteral("foo");
    const QString userId = QStringLiteral("user");
    job.setUserId(userId);
    job.setAuthToken(authToken);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/settings.public")));
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
}

#include "moc_publicsettingsjobtest.cpp"
