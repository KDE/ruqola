/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionupdatejobtest.h"
#include "permissions/permissionupdatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(PermissionUpdateJobTest)
using namespace RocketChatRestApi;
PermissionUpdateJobTest::PermissionUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionUpdateJobTest::shouldHaveDefaultValue()
{
    PermissionUpdateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.permissions().isEmpty());
}

void PermissionUpdateJobTest::shouldGenerateRequest()
{
    PermissionUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/permissions.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void PermissionUpdateJobTest::shouldGenerateJson()
{
    PermissionUpdateJob job;
    QMap<QString, QStringList> lst;
    lst.insert(QStringLiteral("bla"), {QStringLiteral("user"), QStringLiteral("admin")});
    lst.insert(QStringLiteral("team"), {QStringLiteral("user"), QStringLiteral("admin"), QStringLiteral("owner")});
    job.setPermissions(lst);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"permissions":[{"_id":"bla","roles":["user","admin"]},{"_id":"team","roles":["user","admin","owner"]}]})").toLatin1());
}

void PermissionUpdateJobTest::shouldNotStarting()
{
    PermissionUpdateJob job;

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
    QMap<QString, QStringList> lst;
    lst.insert(QStringLiteral("bla"), {QStringLiteral("user"), QStringLiteral("admin")});
    lst.insert(QStringLiteral("team"), {QStringLiteral("user"), QStringLiteral("admin"), QStringLiteral("owner")});
    job.setPermissions(lst);
    QVERIFY(job.canStart());
}

#include "moc_permissionupdatejobtest.cpp"
