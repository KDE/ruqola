/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addusertorolejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "role/addusertorolejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>

QTEST_GUILESS_MAIN(AddUserToRoleJobTest)
using namespace RocketChatRestApi;
AddUserToRoleJobTest::AddUserToRoleJobTest(QObject *parent)
    : QObject(parent)
{
}

void AddUserToRoleJobTest::shouldHaveDefaultValue()
{
    AddUserToRoleJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.username().isEmpty());
}

void AddUserToRoleJobTest::shouldGenerateRequest()
{
    AddUserToRoleJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/roles.addUserToRole"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void AddUserToRoleJobTest::shouldGenerateJson()
{
    AddUserToRoleJob job;

    const QString username = u"foo1"_s;
    job.setUsername(username);
    const QString rolename = u"role1"_s;
    job.setRoleName(rolename);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roleName":"%1","username":"%2"})").arg(rolename, username).toLatin1());
}

void AddUserToRoleJobTest::shouldNotStarting()
{
    AddUserToRoleJob job;

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
}

#include "moc_addusertorolejobtest.cpp"
