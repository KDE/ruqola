/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuscreatetestjob.h"
#include "custom/customuserstatuscreatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(CustomUserStatusCreateTestJob)
using namespace RocketChatRestApi;
CustomUserStatusCreateTestJob::CustomUserStatusCreateTestJob(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusCreateTestJob::shouldHaveDefaultValue()
{
    CustomUserStatusCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomUserStatusCreateTestJob::shouldGenerateRequest()
{
    CustomUserStatusCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void CustomUserStatusCreateTestJob::shouldGenerateJson()
{
    CustomUserStatusCreateJob job;

    const QString name = QStringLiteral("foo1");
    const QString statusType = QStringLiteral("topic1");
    CustomUserStatusCreateJob::StatusCreateInfo info;
    info.name = name;
    info.statusType = statusType;
    job.setStatusCreateInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1","statusType":"%2"})").arg(name, statusType).toLatin1());
}

void CustomUserStatusCreateTestJob::shouldNotStarting()
{
    CustomUserStatusCreateJob job;

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

    CustomUserStatusCreateJob::StatusCreateInfo info;
    info.name = QStringLiteral("foo");
    job.setStatusCreateInfo(info);
    QVERIFY(!job.canStart());
    info.statusType = QStringLiteral("bla");
    job.setStatusCreateInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_customuserstatuscreatetestjob.cpp"
