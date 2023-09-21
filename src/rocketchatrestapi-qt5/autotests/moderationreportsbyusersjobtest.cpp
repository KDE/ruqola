/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsbyusersjobtest.h"
#include "moderation/moderationreportsbyusersjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(ModerationReportsByUsersJobTest)
using namespace RocketChatRestApi;
ModerationReportsByUsersJobTest::ModerationReportsByUsersJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportsByUsersJobTest::shouldHaveDefaultValue()
{
    ModerationReportsByUsersJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(!job.moderationReportsByUsersInfo().isValid());
}

void ModerationReportsByUsersJobTest::shouldGenerateRequest()
{
    ModerationReportsByUsersJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.reportsByUsers")));
    }
}

#include "moc_moderationreportsbyusersjobtest.cpp"
