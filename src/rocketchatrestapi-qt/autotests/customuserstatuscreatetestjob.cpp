/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuscreatetestjob.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomUserStatusCreateTestJob::shouldGenerateRequest()
{
    CustomUserStatusCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-user-status.create"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void CustomUserStatusCreateTestJob::shouldGenerateJson()
{
    CustomUserStatusCreateJob job;

    const QString name = u"foo1"_s;
    const QString statusType = u"topic1"_s;
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

    CustomUserStatusCreateJob::StatusCreateInfo info;
    info.name = u"foo"_s;
    job.setStatusCreateInfo(info);
    QVERIFY(!job.canStart());
    info.statusType = u"bla"_s;
    job.setStatusCreateInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_customuserstatuscreatetestjob.cpp"
