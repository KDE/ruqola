/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appcategoriesjobtest.h"
#include "moderation/moderationreportsjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppCategoriesJobTest)
using namespace RocketChatRestApi;
AppCategoriesJobTest::AppCategoriesJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppCategoriesJobTest::shouldHaveDefaultValue()
{
    ModerationReportsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppCategoriesJobTest::shouldGenerateRequest()
{
    ModerationReportsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setMessageId(QByteArrayLiteral("foo"));
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.reports?msgId=foo")));
}

#include "moc_appcategoriesjobtest.cpp"
