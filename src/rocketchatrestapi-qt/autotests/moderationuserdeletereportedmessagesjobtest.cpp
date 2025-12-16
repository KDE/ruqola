/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserdeletereportedmessagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationuserdeletereportedmessagesjob.h"
#include "restapimethod.h"
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

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"userId":"%1"})").arg(QLatin1StringView(userIdForMessages)).toLatin1());
}

void ModerationUserDeleteReportedMessagesJobTest::shouldNotStarting()
{
    ModerationUserDeleteReportedMessagesJob job;

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
}

#include "moc_moderationuserdeletereportedmessagesjobtest.cpp"
