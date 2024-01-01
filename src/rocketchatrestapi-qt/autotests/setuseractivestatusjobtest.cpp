/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setuseractivestatusjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/setuseractivestatusjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SetUserActiveStatusJobTest)
using namespace RocketChatRestApi;
SetUserActiveStatusJobTest::SetUserActiveStatusJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetUserActiveStatusJobTest::shouldHaveDefaultValue()
{
    SetUserActiveStatusJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.activateUserId().isEmpty());
    QVERIFY(job.activate());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetUserActiveStatusJobTest::shouldGenerateRequest()
{
    SetUserActiveStatusJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setActiveStatus")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SetUserActiveStatusJobTest::shouldGenerateJson()
{
    SetUserActiveStatusJob job;
    const QString userId = QStringLiteral("foo1");
    job.setActivateUserId(userId);
    job.setActivate(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"activeStatus":false,"userId":"%1"})").arg(userId).toLatin1());
    job.setActivate(true);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"activeStatus":true,"userId":"%1"})").arg(userId).toLatin1());
}

void SetUserActiveStatusJobTest::shouldNotStarting()
{
    SetUserActiveStatusJob job;

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
    const QString statusUserid = QStringLiteral("foo1");
    job.setActivateUserId(statusUserid);
    QVERIFY(job.canStart());
}

#include "moc_setuseractivestatusjobtest.cpp"
