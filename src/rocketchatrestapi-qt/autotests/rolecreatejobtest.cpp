/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolecreatejobtest.h"
#include "role/rolecreatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(RoleCreateJobTest)
using namespace RocketChatRestApi;
RoleCreateJobTest::RoleCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoleCreateJobTest::shouldHaveDefaultValue()
{
    RoleCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoleCreateJobTest::shouldGenerateRequest()
{
    RoleCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoleCreateJobTest::shouldGenerateJson()
{
    RoleCreateJob job;

    const QString name = QStringLiteral("foo1");
    const QString description = QStringLiteral("topic1");
    const QString scope = QStringLiteral("Users");
    RoleCreateJob::RoleCreateInfo info;
    info.name = name;
    info.description = description;
    info.scope = scope;
    job.setCreateRoleInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"description":"%3","mandatory2fa":false,"name":"%1","scope":"%2"})").arg(name, scope, description).toLatin1());

    info.mandatory2fa = true;
    job.setCreateRoleInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"description":"%3","mandatory2fa":true,"name":"%1","scope":"%2"})").arg(name, scope, description).toLatin1());
}

void RoleCreateJobTest::shouldNotStarting()
{
    RoleCreateJob job;

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

    RoleCreateJob::RoleCreateInfo info;
    info.name = QStringLiteral("foo");
    job.setCreateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.description = QStringLiteral("bla");
    job.setCreateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.scope = QStringLiteral("bli");
    job.setCreateRoleInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_rolecreatejobtest.cpp"
