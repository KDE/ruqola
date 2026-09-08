/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatusdeletetestjob.h"
using namespace Qt::Literals::StringLiterals;

#include "custom/customuserstatusdeletejob.h"
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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"customUserStatusId":"%1"})"_s.arg(QLatin1StringView(statusId)).toLatin1());
}

void CustomUserStatusDeleteTestJob::shouldNotStarting()
{
    CustomUserStatusDeleteJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);

    const QByteArray statusId("foo1");
    job.setCustomUserStatusId(statusId);
    QVERIFY(job.canStart());
}

#include "moc_customuserstatusdeletetestjob.cpp"
