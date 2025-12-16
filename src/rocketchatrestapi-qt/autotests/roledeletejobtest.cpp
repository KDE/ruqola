/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roledeletejobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.roleId().isEmpty());
}

void RoleDeleteJobTest::shouldGenerateRequest()
{
    RoleDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/roles.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoleDeleteJobTest::shouldGenerateJson()
{
    RoleDeleteJob job;

    const QString identifier = u"foo1"_s;
    job.setRoleId(identifier);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roleId":"%1"})").arg(identifier).toLatin1());
}

void RoleDeleteJobTest::shouldNotStarting()
{
    RoleDeleteJob job;

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
    job.setRoleId(u"ss"_s);
    QVERIFY(job.canStart());
}

#include "moc_roledeletejobtest.cpp"
