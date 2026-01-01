/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "stdoutqueuejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/stdoutqueuejob.h"

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QTest>

QTEST_GUILESS_MAIN(StdoutQueueJobTest)
using namespace RocketChatRestApi;
StdoutQueueJobTest::StdoutQueueJobTest(QObject *parent)
    : QObject(parent)
{
}

void StdoutQueueJobTest::shouldHaveDefaultValue()
{
    StdoutQueueJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void StdoutQueueJobTest::shouldGenerateRequest()
{
    StdoutQueueJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/stdout.queue"_s));
    }
}

#include "moc_stdoutqueuejobtest.cpp"
