/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagejobtest.h"
#include "chat/reportmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <restapimethod.h>

QTEST_GUILESS_MAIN(ReportMessageJobTest)
using namespace RocketChatRestApi;
ReportMessageJobTest::ReportMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void ReportMessageJobTest::shouldHaveDefaultValue()
{
    ReportMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.reportMessage().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ReportMessageJobTest::shouldHaveMessageId()
{
    ReportMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setReportMessage(QStringLiteral("bla"));
    QVERIFY(!job.canStart());
    job.setMessageId(QStringLiteral("bla"));
    QVERIFY(job.canStart());
}

void ReportMessageJobTest::shouldGenerateJobRequest()
{
    ReportMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.reportMessage")));
}

void ReportMessageJobTest::shouldGenerateJson()
{
    ReportMessageJob job;
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    const QString reportMessage = QStringLiteral("foo2");
    job.setReportMessage(reportMessage);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"description":"%2","messageId":"%1"})").arg(messageId, reportMessage).toLatin1());
}

#include "moc_reportmessagejobtest.cpp"
