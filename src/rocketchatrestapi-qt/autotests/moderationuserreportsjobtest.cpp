/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserreportsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationuserreportsjob.h"

#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(ModerationUserReportsJobTest)
using namespace RocketChatRestApi;
ModerationUserReportsJobTest::ModerationUserReportsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationUserReportsJobTest::shouldHaveDefaultValue()
{
    ModerationUserReportsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void ModerationUserReportsJobTest::shouldGenerateRequest()
{
    ModerationUserReportsJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.userReports"_s));
    }
}

void ModerationUserReportsJobTest::shouldNotStarting()
{
    ModerationUserReportsJob job;

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
    QVERIFY(job.canStart());
}

#include "moc_moderationuserreportsjobtest.cpp"
