/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolecreatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "role/rolecreatejob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RoleCreateJobTest)
using namespace RocketChatRestApi;
RoleCreateJobTest::RoleCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoleCreateJobTest::shouldHaveDefaultValue()
{
    RoleCreateJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoleCreateJobTest::shouldGenerateRequest()
{
    RoleCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/roles.create"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoleCreateJobTest::shouldGenerateJson()
{
    RoleCreateJob job;

    const QString name = u"foo1"_s;
    const QString description = u"topic1"_s;
    const QString scope = u"Users"_s;
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

    RoleCreateJob::RoleCreateInfo info;
    info.name = u"foo"_s;
    job.setCreateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.description = u"bla"_s;
    job.setCreateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.scope = u"bli"_s;
    job.setCreateRoleInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_rolecreatejobtest.cpp"
