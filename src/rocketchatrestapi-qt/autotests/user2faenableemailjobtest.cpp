/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2faenableemailjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "2fa/user2faenableemailjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(User2FAEnableEmailJobTest)
using namespace RocketChatRestApi;
User2FAEnableEmailJobTest::User2FAEnableEmailJobTest(QObject *parent)
    : QObject(parent)
{
}

void User2FAEnableEmailJobTest::shouldHaveDefaultValue()
{
    User2FAEnableEmailJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void User2FAEnableEmailJobTest::shouldGenerateRequest()
{
    User2FAEnableEmailJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.2fa.enableEmail"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
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

#include "moc_user2faenableemailjobtest.cpp"
