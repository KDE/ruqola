/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsjobtest.h"
#include "moderation/moderationreportsjob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(ModerationReportsJobTest)
using namespace RocketChatRestApi;
ModerationReportsJobTest::ModerationReportsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportsJobTest::shouldHaveDefaultValue()
{
    ModerationReportsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ModerationReportsJobTest::shouldGenerateRequest()
{
    ModerationReportsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setMessageId(QStringLiteral("foo"));
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.reports?msgId=foo")));
}

#include "moc_moderationreportsjobtest.cpp"
