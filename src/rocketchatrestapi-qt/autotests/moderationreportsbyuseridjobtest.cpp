/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsbyuseridjobtest.h"
#include "moderation/moderationreportsbyuseridjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(ModerationReportsByUserIdJobTest)
using namespace RocketChatRestApi;
ModerationReportsByUserIdJobTest::ModerationReportsByUserIdJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportsByUserIdJobTest::shouldHaveDefaultValue()
{
    ModerationReportsByUserIdJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.moderationUserId().isEmpty());
}

void ModerationReportsByUserIdJobTest::shouldGenerateRequest()
{
    {
        ModerationReportsByUserIdJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.user.reportsByUserId?userId")));
    }
    {
        ModerationReportsByUserIdJob job;
        job.setModerationUserId("foo"_ba);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.user.reportsByUserId?userId=foo")));
    }
}

#include "moc_moderationreportsbyuseridjobtest.cpp"
