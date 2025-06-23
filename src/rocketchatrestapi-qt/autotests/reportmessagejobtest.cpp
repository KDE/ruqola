/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/reportmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setReportMessage(u"bla"_s);
    QVERIFY(!job.canStart());
    job.setMessageId("sbla"_ba);
    QVERIFY(job.canStart());
}

void ReportMessageJobTest::shouldGenerateJobRequest()
{
    ReportMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.reportMessage"_s));
}

void ReportMessageJobTest::shouldGenerateJson()
{
    ReportMessageJob job;
    const QByteArray messageId("foo1");
    job.setMessageId(messageId);
    const QString reportMessage = u"foo2"_s;
    job.setReportMessage(reportMessage);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"description":"%2","messageId":"%1"})").arg(QLatin1StringView(messageId), reportMessage).toLatin1());
}

#include "moc_reportmessagejobtest.cpp"
