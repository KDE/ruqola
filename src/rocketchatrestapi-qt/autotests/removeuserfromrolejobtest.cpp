/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeuserfromrolejobtest.h"
#include "role/removeuserfromrolejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(RemoveUserFromRoleJobTest)
using namespace RocketChatRestApi;
RemoveUserFromRoleJobTest::RemoveUserFromRoleJobTest(QObject *parent)
    : QObject(parent)
{
}

void RemoveUserFromRoleJobTest::shouldHaveDefaultValue()
{
    RemoveUserFromRoleJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RemoveUserFromRoleJobTest::shouldGenerateRequest()
{
    RemoveUserFromRoleJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/roles.removeUserFromRole")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RemoveUserFromRoleJobTest::shouldGenerateJson()
{
    RemoveUserFromRoleJob job;

    const QString username = QStringLiteral("foo1");
    job.setUsername(username);
    const QString rolename = QStringLiteral("role1");
    job.setRoleName(rolename);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roleName":"%1","username":"%2"})").arg(rolename, username).toLatin1());
}

void RemoveUserFromRoleJobTest::shouldNotStarting()
{
    RemoveUserFromRoleJob job;

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
    const QString username = QStringLiteral("foo1");
    job.setUsername(username);
    QVERIFY(!job.canStart());
    const QString rolename = QStringLiteral("role1");
    job.setRoleName(rolename);
    QVERIFY(job.canStart());
}

#include "moc_removeuserfromrolejobtest.cpp"
