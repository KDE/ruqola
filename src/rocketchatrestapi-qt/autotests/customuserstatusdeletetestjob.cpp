/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatusdeletetestjob.h"
#include "custom/customuserstatusdeletejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(CustomUserStatusDeleteTestJob)
using namespace RocketChatRestApi;
CustomUserStatusDeleteTestJob::CustomUserStatusDeleteTestJob(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusDeleteTestJob::shouldHaveDefaultValue()
{
    CustomUserStatusDeleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomUserStatusDeleteTestJob::shouldGenerateRequest()
{
    CustomUserStatusDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void CustomUserStatusDeleteTestJob::shouldGenerateJson()
{
    CustomUserStatusDeleteJob job;

    const QString statusId = QStringLiteral("foo1");
    job.setCustomUserStatusId(statusId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"customUserStatusId":"%1"})").arg(statusId).toLatin1());
}

void CustomUserStatusDeleteTestJob::shouldNotStarting()
{
    CustomUserStatusDeleteJob job;

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

    const QString statusId = QStringLiteral("foo1");
    job.setCustomUserStatusId(statusId);
    QVERIFY(job.canStart());
}

#include "moc_customuserstatusdeletetestjob.cpp"
