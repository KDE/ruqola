/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissuserreportsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationdismissuserreportsjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ModerationDismissUserReportsJobTest)
using namespace RocketChatRestApi;
ModerationDismissUserReportsJobTest::ModerationDismissUserReportsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationDismissUserReportsJobTest::shouldHaveDefaultValue()
{
    ModerationDismissUserReportsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.moderationReportedUserId().isEmpty());
}

void ModerationDismissUserReportsJobTest::shouldGenerateRequest()
{
    ModerationDismissUserReportsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.dismissUserReports"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ModerationDismissUserReportsJobTest::shouldGenerateJson()
{
    ModerationDismissUserReportsJob job;
    job.setModerationReportedUserId("foo"_ba);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), R"({"userId":"foo"})"_ba);
}

void ModerationDismissUserReportsJobTest::shouldNotStarting()
{
    ModerationDismissUserReportsJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    job.setModerationReportedUserId("foo1"_ba);
    QVERIFY(job.canStart());
}

#include "moc_moderationdismissuserreportsjobtest.cpp"
