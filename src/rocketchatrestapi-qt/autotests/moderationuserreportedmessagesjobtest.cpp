/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserreportedmessagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationuserreportedmessagesjob.h"

#include "restapimethod.h"
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

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QByteArray userIdFromMessages("foo1");
    job.setReportedMessageFromUserId(userIdFromMessages);
    QVERIFY(job.canStart());
}

#include "moc_moderationuserreportedmessagesjobtest.cpp"
