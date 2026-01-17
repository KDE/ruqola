/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeuserfromrolejobtest.h"

#include "role/removeuserfromrolejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
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
    QVERIFY(job.roleId().isEmpty());
    QVERIFY(job.roleName().isEmpty());
    QVERIFY(!job.useRC80());
}

void RemoveUserFromRoleJobTest::shouldGenerateRequest()
{
    RemoveUserFromRoleJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/roles.removeUserFromRole"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RemoveUserFromRoleJobTest::shouldGenerateJson()
{
    {
        RemoveUserFromRoleJob job;
        job.setUseRC80(false);

        const QString username = u"foo1"_s;
        job.setUsername(username);
        const QString rolename = u"role1"_s;
        job.setRoleName(rolename);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roleName":"%1","username":"%2"})").arg(rolename, username).toLatin1());
    }
    {
        RemoveUserFromRoleJob job;
        job.setUseRC80(true);

        const QString username = u"foo1"_s;
        job.setUsername(username);
        const QString rolename = u"role1"_s;
        job.setRoleName(rolename);
        const QString roleId = u"roleId1"_s;
        job.setRoleId(roleId);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roleId":"%1","username":"%2"})").arg(roleId, username).toLatin1());
    }
}

void RemoveUserFromRoleJobTest::shouldNotStarting()
{
    RemoveUserFromRoleJob job;
    job.setUseRC80(false);

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
    const QString username = u"foo1"_s;
    job.setUsername(username);
    QVERIFY(!job.canStart());
    const QString rolename = u"role1"_s;
    job.setRoleName(rolename);
    QVERIFY(job.canStart());

    job.setUseRC80(true);
    QVERIFY(!job.canStart());
    const QString roleId = u"roleId1"_s;
    job.setRoleId(roleId);
    QVERIFY(job.canStart());
}

#include "moc_removeuserfromrolejobtest.cpp"
