/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserreportsjobtest.h"
#include "moderation/moderationuserreportsjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(ModerationUserReportsJobTest)
using namespace RocketChatRestApi;
ModerationUserReportsJobTest::ModerationUserReportsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationUserReportsJobTest::shouldHaveDefaultValue()
{
    ModerationUserReportsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.reportedMessageFromUserId().isEmpty());
}

void ModerationUserReportsJobTest::shouldGenerateRequest()
{
    ModerationUserReportsJob job;
    job.setReportedMessageFromUserId(QStringLiteral("bla"));
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.user.reportedMessages?userId=bla")));
    }
}

void ModerationUserReportsJobTest::shouldNotStarting()
{
    ModerationUserReportsJob job;

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

#include "moc_moderationuserreportsjobtest.cpp"
