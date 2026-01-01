/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionupdatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "permissions/permissionupdatejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(PermissionUpdateJobTest)
using namespace RocketChatRestApi;
PermissionUpdateJobTest::PermissionUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionUpdateJobTest::shouldHaveDefaultValue()
{
    PermissionUpdateJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.permissions().isEmpty());
}

void PermissionUpdateJobTest::shouldGenerateRequest()
{
    PermissionUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/permissions.update"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void PermissionUpdateJobTest::shouldGenerateJson()
{
    PermissionUpdateJob job;
    QMap<QString, QStringList> lst;
    lst.insert(u"bla"_s, {u"user"_s, u"admin"_s});
    lst.insert(u"team"_s, {u"user"_s, u"admin"_s, u"owner"_s});
    job.setPermissions(lst);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"permissions":[{"_id":"bla","roles":["user","admin"]},{"_id":"team","roles":["user","admin","owner"]}]})").toLatin1());
}

void PermissionUpdateJobTest::shouldNotStarting()
{
    PermissionUpdateJob job;

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
    QMap<QString, QStringList> lst;
    lst.insert(u"bla"_s, {u"user"_s, u"admin"_s});
    lst.insert(u"team"_s, {u"user"_s, u"admin"_s, u"owner"_s});
    job.setPermissions(lst);
    QVERIFY(job.canStart());
}

#include "moc_permissionupdatejobtest.cpp"
