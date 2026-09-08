/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserreportedmessagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationuserreportedmessagesjob.h"

#include "ruqola_restapi_helper.h"
#include <QTest>

QTEST_GUILESS_MAIN(ModerationUserReportedMessagesJobTest)
using namespace RocketChatRestApi;
ModerationUserReportedMessagesJobTest::ModerationUserReportedMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationUserReportedMessagesJobTest::shouldHaveDefaultValue()
{
    ModerationUserReportedMessagesJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.reportedMessageFromUserId().isEmpty());
}

void ModerationUserReportedMessagesJobTest::shouldGenerateRequest()
{
    ModerationUserReportedMessagesJob job;
    job.setReportedMessageFromUserId("bla"_ba);
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.user.reportedMessages?userId=bla"_s));
    }
}

void ModerationUserReportedMessagesJobTest::shouldNotStarting()
{
    ModerationUserReportedMessagesJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray userIdFromMessages("foo1");
    job.setReportedMessageFromUserId(userIdFromMessages);
    QVERIFY(job.canStart());
}

#include "moc_moderationuserreportedmessagesjobtest.cpp"
