/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "validateinvitetokenjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "invite/validateinvitetokenjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ValidateInviteTokenJobTest)
using namespace RocketChatRestApi;
ValidateInviteTokenJobTest::ValidateInviteTokenJobTest(QObject *parent)
    : QObject(parent)
{
}

void ValidateInviteTokenJobTest::shouldHaveDefaultValue()
{
    ValidateInviteTokenJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.token().isEmpty());
}

void ValidateInviteTokenJobTest::shouldGenerateRequest()
{
    ValidateInviteTokenJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/validateInviteToken"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ValidateInviteTokenJobTest::shouldGenerateJson()
{
    ValidateInviteTokenJob job;
    job.setToken(u"bla"_s);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"token":"bla"})").toLatin1());
}

void ValidateInviteTokenJobTest::shouldNotStarting()
{
    ValidateInviteTokenJob job;

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
    job.setToken(u"bla1"_s);
    QVERIFY(job.canStart());
}

#include "moc_validateinvitetokenjobtest.cpp"
