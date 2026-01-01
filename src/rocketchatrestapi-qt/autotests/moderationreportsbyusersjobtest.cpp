/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsbyusersjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationreportsbyusersjob.h"

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

QTEST_GUILESS_MAIN(ModerationReportsByUsersJobTest)
using namespace RocketChatRestApi;
ModerationReportsByUsersJobTest::ModerationReportsByUsersJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportsByUsersJobTest::shouldHaveDefaultValue()
{
    ModerationReportsByUsersJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(!job.moderationReportsByUsersInfo().isValid());
}

void ModerationReportsByUsersJobTest::shouldGenerateRequest()
{
    {
        ModerationReportsByUsersJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.reportsByUsers"_s));
    }
    {
        ModerationReportsByUsersJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        ModerationReportsByUsersJob::ModerationReportsByUsersInfo info;
        info.mLatest = QDateTime(QDate(2023, 1, 5), QTime(5, 10, 3));
        info.mOldest = QDateTime(QDate(2022, 1, 5), QTime(5, 10, 3));
        job.setModerationReportsByUsersInfo(info);
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.reportsByUsers?oldest=2022-01-05T05:10:03&latest=2023-01-05T05:10:03"_s));
    }
}

#include "moc_moderationreportsbyusersjobtest.cpp"
