/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sendinvitationemailjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "invite/sendinvitationemailjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(SendInvitationEmailJobTest)
using namespace RocketChatRestApi;
SendInvitationEmailJobTest::SendInvitationEmailJobTest(QObject *parent)
    : QObject(parent)
{
}

void SendInvitationEmailJobTest::shouldHaveDefaultValue()
{
    SendInvitationEmailJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.emails().isEmpty());
}

void SendInvitationEmailJobTest::shouldGenerateRequest()
{
    SendInvitationEmailJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/sendInvitationEmail"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SendInvitationEmailJobTest::shouldGenerateJson()
{
    SendInvitationEmailJob job;
    QStringList emails;
    emails.append(u"bla@kde.org"_s);
    emails.append(u"bli@kde.org"_s);
    job.setEmails(emails);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emails":["bla@kde.org","bli@kde.org"]})").toLatin1());
}

void SendInvitationEmailJobTest::shouldNotStarting()
{
    SendInvitationEmailJob job;

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
    const QStringList emails{u"foo"_s, u"bla"_s};
    job.setEmails(emails);
    QVERIFY(job.canStart());
}

#include "moc_sendinvitationemailjobtest.cpp"
