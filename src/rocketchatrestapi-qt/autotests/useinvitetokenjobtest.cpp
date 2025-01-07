/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "useinvitetokenjobtest.h"
#include "invite/useinvitetokenjob.h"
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
    UseInviteTokenJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.token().isEmpty());
}

void UseInviteTokenJobTest::shouldGenerateRequest()
{
    UseInviteTokenJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/useInviteToken")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UseInviteTokenJobTest::shouldGenerateJson()
{
    UseInviteTokenJob job;
    job.setToken(QStringLiteral("bla"));
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"token":"bla"})").toLatin1());
}

void UseInviteTokenJobTest::shouldNotStarting()
{
    UseInviteTokenJob job;

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
    job.setToken(QStringLiteral("bla"));
    QVERIFY(job.canStart());
}

#include "moc_useinvitetokenjobtest.cpp"
