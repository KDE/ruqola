/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissreportsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationdismissreportsjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ModerationDismissReportsJobTest)
using namespace RocketChatRestApi;
ModerationDismissReportsJobTest::ModerationDismissReportsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationDismissReportsJobTest::shouldHaveDefaultValue()
{
    ModerationDismissReportsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.userIdForMessages().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.userIdForMessages().isEmpty());
}

void ModerationDismissReportsJobTest::shouldGenerateRequest()
{
    ModerationDismissReportsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.dismissReports"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ModerationDismissReportsJobTest::shouldGenerateJson()
{
    {
        ModerationDismissReportsJob job;
        const QByteArray userIdForMessages("foo2");
        job.setUserIdForMessages(userIdForMessages);

        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"userId":"%1"})").arg(QLatin1StringView(userIdForMessages)).toLatin1());
    }
    {
        ModerationDismissReportsJob job;
        const QByteArray messageId("foo7");
        job.setMessageId(messageId);

        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"msgId":"%1"})").arg(QLatin1StringView(messageId)).toLatin1());
    }
}

void ModerationDismissReportsJobTest::shouldNotStarting()
{
    ModerationDismissReportsJob job;

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
    const QByteArray userIdForMessages("foo1");
    job.setUserIdForMessages(userIdForMessages);
    QVERIFY(job.canStart());
    job.setUserIdForMessages(QByteArray());
    QVERIFY(!job.canStart());
    const QByteArray messageId("foo2");
    job.setMessageId(messageId);
    QVERIFY(job.canStart());
}

#include "moc_moderationdismissreportsjobtest.cpp"
