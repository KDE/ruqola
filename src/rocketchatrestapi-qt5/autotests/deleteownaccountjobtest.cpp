/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deleteownaccountjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/deleteownaccountjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(DeleteOwnAccountJobTest)
using namespace RocketChatRestApi;
DeleteOwnAccountJobTest::DeleteOwnAccountJobTest(QObject *parent)
    : QObject(parent)
{
}

void DeleteOwnAccountJobTest::shouldHaveDefaultValue()
{
    DeleteOwnAccountJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.password().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void DeleteOwnAccountJobTest::shouldGenerateRequest()
{
    DeleteOwnAccountJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.deleteOwnAccount")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void DeleteOwnAccountJobTest::shouldGenerateJson()
{
    DeleteOwnAccountJob job;
    const QString password = QStringLiteral("foo1");
    job.setPassword(password);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"password":"bb4eca334f61af3b67b5d528907d30285151610200539302f4c8cabe66225b53"})").toLatin1());
}

void DeleteOwnAccountJobTest::shouldNotStarting()
{
    DeleteOwnAccountJob job;

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
    const QString password = QStringLiteral("foo1");
    job.setPassword(password);
    QVERIFY(job.canStart());
}
