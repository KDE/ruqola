/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserreportedmessagesjobtest.h"
#include "moderation/moderationuserreportedmessagesjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(ModerationUserReportedMessagesJobTest)
using namespace RocketChatRestApi;
ModerationUserReportedMessagesJobTest::ModerationUserReportedMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationUserReportedMessagesJobTest::shouldHaveDefaultValue()
{
    ModerationUserReportedMessagesJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.reportedMessageFromUserId().isEmpty());
}

void ModerationUserReportedMessagesJobTest::shouldGenerateRequest()
{
    ModerationUserReportedMessagesJob job;
    job.setReportedMessageFromUserId(QStringLiteral("bla"));
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.user.reportedMessages?userId=bla")));
    }
}

void ModerationUserReportedMessagesJobTest::shouldNotStarting()
{
    ModerationUserReportedMessagesJob job;

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
    QVERIFY(!job.canStart());
    const QString userIdFromMessages = QStringLiteral("foo1");
    job.setReportedMessageFromUserId(userIdFromMessages);
    QVERIFY(job.canStart());
}

#include "moc_moderationuserreportedmessagesjobtest.cpp"
