/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatusupdatetestjob.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "custom/customuserstatusupdatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(CustomUserStatusUpdateTestJob)
using namespace RocketChatRestApi;
CustomUserStatusUpdateTestJob::CustomUserStatusUpdateTestJob(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusUpdateTestJob::shouldHaveDefaultValue()
{
    CustomUserStatusUpdateJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomUserStatusUpdateTestJob::shouldGenerateRequest()
{
    CustomUserStatusUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-user-status.update"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void CustomUserStatusUpdateTestJob::shouldGenerateJson()
{
    CustomUserStatusUpdateJob job;

    const QString name = u"foo1"_s;
    const QString statusType = u"topic1"_s;
    const QByteArray identifier = "id1"_ba;
    CustomUserStatusUpdateJob::StatusUpdateInfo info;
    info.name = name;
    info.statusType = statusType;
    info.identifier = identifier;
    job.setStatusUpdateInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"_id":"%3","name":"%1","statusType":"%2"})").arg(name, statusType, QLatin1StringView(identifier)).toLatin1());
}

void CustomUserStatusUpdateTestJob::shouldNotStarting()
{
    CustomUserStatusUpdateJob job;

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

    const QString name = u"foo1"_s;
    const QString statusType = u"topic1"_s;
    const QByteArray identifier = "id1"_ba;

    CustomUserStatusUpdateJob::StatusUpdateInfo info;
    info.name = name;
    job.setStatusUpdateInfo(info);
    QVERIFY(!job.canStart());
    info.statusType = statusType;
    job.setStatusUpdateInfo(info);
    QVERIFY(!job.canStart());
    info.identifier = identifier;
    job.setStatusUpdateInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_customuserstatusupdatetestjob.cpp"
