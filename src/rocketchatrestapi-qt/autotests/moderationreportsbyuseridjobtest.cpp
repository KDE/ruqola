/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsbyuseridjobtest.h"
#include "moderation/moderationreportsbyusersjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(ModerationReportsByUserIdJobTest)
using namespace RocketChatRestApi;
ModerationReportsByUserIdJobTest::ModerationReportsByUserIdJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportsByUserIdJobTest::shouldHaveDefaultValue()
{
    ModerationReportsByUsersJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(!job.moderationReportsByUsersInfo().isValid());
}

void ModerationReportsByUserIdJobTest::shouldGenerateRequest()
{
    ModerationReportsByUsersJob job;
    {
        ModerationReportsByUsersJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.reportsByUsers")));
    }
    {
        ModerationReportsByUsersJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        ModerationReportsByUsersJob::ModerationReportsByUsersInfo info;
        info.mLatest = QDateTime(QDate(2023, 1, 5), QTime(5, 10, 3));
        info.mOldest = QDateTime(QDate(2022, 1, 5), QTime(5, 10, 3));
        job.setModerationReportsByUsersInfo(info);
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(),
                 QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.reportsByUsers?oldest=2022-01-05T05:10:03&latest=2023-01-05T05:10:03")));
    }
}

#include "moc_moderationreportsbyuseridjobtest.cpp"
