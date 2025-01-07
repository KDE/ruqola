/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "validateinvitetokenjobtest.h"
#include "invite/validateinvitetokenjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ValidateInviteTokenJobTest)
using namespace RocketChatRestApi;
ValidateInviteTokenJobTest::ValidateInviteTokenJobTest(QObject *parent)
    : QObject(parent)
{
}

void ValidateInviteTokenJobTest::shouldHaveDefaultValue()
{
    ValidateInviteTokenJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.token().isEmpty());
}

void ValidateInviteTokenJobTest::shouldGenerateRequest()
{
    ValidateInviteTokenJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/validateInviteToken")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ValidateInviteTokenJobTest::shouldGenerateJson()
{
    ValidateInviteTokenJob job;
    job.setToken(QStringLiteral("bla"));
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"token":"bla"})").toLatin1());
}

void ValidateInviteTokenJobTest::shouldNotStarting()
{
    ValidateInviteTokenJob job;

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
    job.setToken(QStringLiteral("bla1"));
    QVERIFY(job.canStart());
}

#include "moc_validateinvitetokenjobtest.cpp"
