/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roledeletejobtest.h"
#include "role/roledeletejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(RoleDeleteJobTest)
using namespace RocketChatRestApi;
RoleDeleteJobTest::RoleDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoleDeleteJobTest::shouldHaveDefaultValue()
{
    RoleDeleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoleDeleteJobTest::shouldGenerateRequest()
{
    RoleDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoleDeleteJobTest::shouldGenerateJson()
{
    RoleDeleteJob job;

    const QString identifier = QStringLiteral("foo1");
    job.setRoleId(identifier);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roleId":"%1"})").arg(identifier).toLatin1());
}

void RoleDeleteJobTest::shouldNotStarting()
{
    RoleDeleteJob job;

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
    job.setRoleId(QStringLiteral("ss"));
    QVERIFY(job.canStart());
}
