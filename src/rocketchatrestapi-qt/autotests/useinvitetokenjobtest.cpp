/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "useinvitetokenjobtest.h"
#include "invite/sendinvitationemailjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UseInviteTokenJobTest)
using namespace RocketChatRestApi;
UseInviteTokenJobTest::UseInviteTokenJobTest(QObject *parent)
    : QObject(parent)
{
}

void UseInviteTokenJobTest::shouldHaveDefaultValue()
{
    SendInvitationEmailJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.emails().isEmpty());
}

void UseInviteTokenJobTest::shouldGenerateRequest()
{
    SendInvitationEmailJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/sendInvitationEmail")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UseInviteTokenJobTest::shouldGenerateJson()
{
    SendInvitationEmailJob job;
    QStringList emails;
    emails.append(QStringLiteral("bla@kde.org"));
    emails.append(QStringLiteral("bli@kde.org"));
    job.setEmails(emails);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emails":["bla@kde.org","bli@kde.org"]})").toLatin1());
}

void UseInviteTokenJobTest::shouldNotStarting()
{
    SendInvitationEmailJob job;

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
    const QStringList emails{QStringLiteral("foo"), QStringLiteral("bla")};
    job.setEmails(emails);
    QVERIFY(job.canStart());
}

#include "moc_useinvitetokenjobtest.cpp"
