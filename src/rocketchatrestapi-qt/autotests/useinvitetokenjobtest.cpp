/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "useinvitetokenjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "invite/useinvitetokenjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(UseInviteTokenJobTest)
using namespace RocketChatRestApi;
UseInviteTokenJobTest::UseInviteTokenJobTest(QObject *parent)
    : QObject(parent)
{
}

void UseInviteTokenJobTest::shouldHaveDefaultValue()
{
    UseInviteTokenJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.token().isEmpty());
}

void UseInviteTokenJobTest::shouldGenerateRequest()
{
    UseInviteTokenJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/useInviteToken"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UseInviteTokenJobTest::shouldGenerateJson()
{
    UseInviteTokenJob job;
    job.setToken(u"bla"_s);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"token":"bla"})").toLatin1());
}

void UseInviteTokenJobTest::shouldNotStarting()
{
    UseInviteTokenJob job;

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
    job.setToken(u"bla"_s);
    QVERIFY(job.canStart());
}

#include "moc_useinvitetokenjobtest.cpp"
