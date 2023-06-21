/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resete2ekeyjobtest.h"
#include "restapimethod.h"
#include "users/resete2ekeyjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ResetE2EKeyJobTest)
using namespace RocketChatRestApi;
ResetE2EKeyJobTest::ResetE2EKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void ResetE2EKeyJobTest::shouldHaveDefaultValue()
{
    ResetE2EKeyJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
    QVERIFY(job.resetUserId().isEmpty());
}

void ResetE2EKeyJobTest::shouldGenerateRequest()
{
    ResetE2EKeyJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetE2EKey")));
}

void ResetE2EKeyJobTest::shouldGenerateJson()
{
    ResetE2EKeyJob job;
    const QString resetUserId = QStringLiteral("foo");
    job.setResetUserId(resetUserId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"userId":"%1"})").arg(resetUserId).toLatin1());
}

void ResetE2EKeyJobTest::shouldNotStarting()
{
    ResetE2EKeyJob job;

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

    job.setResetUserId(QStringLiteral("ss"));
    QVERIFY(!job.canStart());

    job.setAuthCode(QStringLiteral("bla"));
    QVERIFY(!job.canStart());

    job.setAuthMethod(QStringLiteral("method"));
    QVERIFY(job.canStart());
}

#include "moc_resete2ekeyjobtest.cpp"
