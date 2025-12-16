/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleupdatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoleUpdateJobTest::shouldGenerateRequest()
{
    RoleUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/roles.update"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoleUpdateJobTest::shouldGenerateJson()
{
    RoleUpdateJob job;

    const QString name = u"foo1"_s;
    const QString description = u"topic1"_s;
    const QString scope = u"Users"_s;
    const QString roleId = u"BLABLA"_s;
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

    RoleUpdateJob::RoleUpdateInfo info;
    info.name = u"foo"_s;
    job.setUpdateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.description = u"bla"_s;
    job.setUpdateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.scope = u"bli"_s;
    job.setUpdateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.identifier = u"bl"_s;
    job.setUpdateRoleInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_roleupdatejobtest.cpp"
