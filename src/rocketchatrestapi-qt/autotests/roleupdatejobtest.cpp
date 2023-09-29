/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleupdatejobtest.h"
#include "role/roleupdatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(RoleUpdateJobTest)
using namespace RocketChatRestApi;
RoleUpdateJobTest::RoleUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoleUpdateJobTest::shouldHaveDefaultValue()
{
    RoleUpdateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoleUpdateJobTest::shouldGenerateRequest()
{
    RoleUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoleUpdateJobTest::shouldGenerateJson()
{
    RoleUpdateJob job;

    const QString name = QStringLiteral("foo1");
    const QString description = QStringLiteral("topic1");
    const QString scope = QStringLiteral("Users");
    const QString roleId = QStringLiteral("BLABLA");
    RoleUpdateJob::RoleUpdateInfo info;
    info.name = name;
    info.description = description;
    info.scope = scope;
    info.identifier = roleId;
    job.setUpdateRoleInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(R"({"description":"%3","mandatory2fa":false,"name":"%1","roleId":"%4","scope":"%2"})").arg(name, scope, description, roleId).toLatin1());
}

void RoleUpdateJobTest::shouldNotStarting()
{
    RoleUpdateJob job;

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

    RoleUpdateJob::RoleUpdateInfo info;
    info.name = QStringLiteral("foo");
    job.setUpdateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.description = QStringLiteral("bla");
    job.setUpdateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.scope = QStringLiteral("bli");
    job.setUpdateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.identifier = QStringLiteral("bl");
    job.setUpdateRoleInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_roleupdatejobtest.cpp"
