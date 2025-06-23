/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    job.setMessageId("foo"_ba);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.reports?msgId=foo"_s));
}

#include "moc_moderationreportinfojobtest.cpp"
