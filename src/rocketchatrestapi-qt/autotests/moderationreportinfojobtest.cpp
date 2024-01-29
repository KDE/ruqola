/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfojobtest.h"
#include "moderation/moderationreportsjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(ModerationReportInfoJobTest)
using namespace RocketChatRestApi;
ModerationReportInfoJobTest::ModerationReportInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportInfoJobTest::shouldHaveDefaultValue()
{
    ModerationReportsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ModerationReportInfoJobTest::shouldGenerateRequest()
{
    ModerationReportsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setMessageId(QStringLiteral("foo"));
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.reports?msgId=foo")));
}

#include "moc_moderationreportinfojobtest.cpp"
