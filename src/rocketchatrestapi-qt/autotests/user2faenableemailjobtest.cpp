/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2faenableemailjobtest.h"
#include "2fa/user2faenableemailjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(User2FAEnableEmailJobTest)
using namespace RocketChatRestApi;
User2FAEnableEmailJobTest::User2FAEnableEmailJobTest(QObject *parent)
    : QObject(parent)
{
}

void User2FAEnableEmailJobTest::shouldHaveDefaultValue()
{
    User2FAEnableEmailJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void User2FAEnableEmailJobTest::shouldGenerateRequest()
{
    User2FAEnableEmailJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.2fa.enableEmail")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void User2FAEnableEmailJobTest::shouldGenerateJson()
{
    User2FAEnableEmailJob job;
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QByteArray("{}"));
}

void User2FAEnableEmailJobTest::shouldNotStarting()
{
    User2FAEnableEmailJob job;

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
    QVERIFY(job.canStart());
}

#include "moc_user2faenableemailjobtest.cpp"
