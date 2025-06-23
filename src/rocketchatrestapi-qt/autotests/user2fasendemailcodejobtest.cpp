/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2fasendemailcodejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "2fa/user2fasendemailcodejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(User2FASendEmailCodeJobTest)
using namespace RocketChatRestApi;
User2FASendEmailCodeJobTest::User2FASendEmailCodeJobTest(QObject *parent)
    : QObject(parent)
{
}

void User2FASendEmailCodeJobTest::shouldHaveDefaultValue()
{
    User2FASendEmailCodeJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void User2FASendEmailCodeJobTest::shouldGenerateRequest()
{
    User2FASendEmailCodeJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.2fa.sendEmailCode"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void User2FASendEmailCodeJobTest::shouldGenerateJson()
{
    User2FASendEmailCodeJob job;
    const QString emails = u"emails"_s;
    job.setUsernameOrEmail(emails);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emailOrUsername":"%1"})").arg(emails).toLatin1());
}

void User2FASendEmailCodeJobTest::shouldNotStarting()
{
    User2FASendEmailCodeJob job;

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
    QVERIFY(!job.canStart());
    job.setUsernameOrEmail(u"emails"_s);
    QVERIFY(job.canStart());
}

#include "moc_user2fasendemailcodejobtest.cpp"
