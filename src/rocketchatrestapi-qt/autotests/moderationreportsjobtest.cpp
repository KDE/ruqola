/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationreportsjob.h"

#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(ModerationReportsJobTest)
using namespace RocketChatRestApi;
ModerationReportsJobTest::ModerationReportsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportsJobTest::shouldHaveDefaultValue()
{
    ModerationReportsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ModerationReportsJobTest::shouldGenerateRequest()
{
    ModerationReportsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setMessageId("foo"_ba);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.reports?msgId=foo"_s));
}

#include "moc_moderationreportsjobtest.cpp"
