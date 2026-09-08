/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserdeletereportedmessagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationuserdeletereportedmessagesjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QJsonDocument>

QTEST_GUILESS_MAIN(ModerationUserDeleteReportedMessagesJobTest)
using namespace RocketChatRestApi;
ModerationUserDeleteReportedMessagesJobTest::ModerationUserDeleteReportedMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationUserDeleteReportedMessagesJobTest::shouldHaveDefaultValue()
{
    ModerationUserDeleteReportedMessagesJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.userIdForMessages().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ModerationUserDeleteReportedMessagesJobTest::shouldGenerateRequest()
{
    ModerationUserDeleteReportedMessagesJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.user.deleteReportedMessages"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ModerationUserDeleteReportedMessagesJobTest::shouldGenerateJson()
{
    ModerationUserDeleteReportedMessagesJob job;
    const QByteArray userIdForMessages("foo2");
    job.setUserIdForMessages(userIdForMessages);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"userId":"%1"})"_s.arg(QLatin1StringView(userIdForMessages)).toLatin1());
}

void ModerationUserDeleteReportedMessagesJobTest::shouldNotStarting()
{
    ModerationUserDeleteReportedMessagesJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray userIdForMessages("foo1");
    job.setUserIdForMessages(userIdForMessages);
    QVERIFY(job.canStart());
}

#include "moc_moderationuserdeletereportedmessagesjobtest.cpp"
