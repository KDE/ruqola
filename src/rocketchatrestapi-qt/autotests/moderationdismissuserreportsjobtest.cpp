/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissuserreportsjobtest.h"
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
    QVERIFY(job.moderationUserId().isEmpty());
}

void ModerationDismissUserReportsJobTest::shouldGenerateRequest()
{
    {
        ModerationDismissUserReportsJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.user.reportsByUserId?userId")));
    }
    {
        ModerationDismissUserReportsJob job;
        job.setModerationUserId(QByteArrayLiteral("foo"));
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.user.reportsByUserId?userId=foo")));
    }
}

#include "moc_moderationdismissuserreportsjobtest.cpp"
