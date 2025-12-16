/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatusdeletetestjob.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "custom/customuserstatusdeletejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(CustomUserStatusDeleteTestJob)
using namespace RocketChatRestApi;
CustomUserStatusDeleteTestJob::CustomUserStatusDeleteTestJob(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusDeleteTestJob::shouldHaveDefaultValue()
{
    CustomUserStatusDeleteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomUserStatusDeleteTestJob::shouldGenerateRequest()
{
    CustomUserStatusDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-user-status.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void CustomUserStatusDeleteTestJob::shouldGenerateJson()
{
    CustomUserStatusDeleteJob job;

    const QByteArray statusId("foo1");
    job.setCustomUserStatusId(statusId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"customUserStatusId":"%1"})").arg(QLatin1StringView(statusId)).toLatin1());
}

void CustomUserStatusDeleteTestJob::shouldNotStarting()
{
    CustomUserStatusDeleteJob job;

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

    const QByteArray statusId("foo1");
    job.setCustomUserStatusId(statusId);
    QVERIFY(job.canStart());
}

#include "moc_customuserstatusdeletetestjob.cpp"
