/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissuserreportsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationdismissuserreportsjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(ModerationDismissUserReportsJobTest)
using namespace RocketChatRestApi;
ModerationDismissUserReportsJobTest::ModerationDismissUserReportsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationDismissUserReportsJobTest::shouldHaveDefaultValue()
{
    ModerationDismissUserReportsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.moderationReportedUserId().isEmpty());
}

void ModerationDismissUserReportsJobTest::shouldGenerateRequest()
{
    {
        ModerationDismissUserReportsJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.dismissUserReports?userId"_s));
    }
    {
        ModerationDismissUserReportsJob job;
        job.setModerationReportedUserId("foo"_ba);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.dismissUserReports?userId=foo"_s));
    }
}

#include "moc_moderationdismissuserreportsjobtest.cpp"
