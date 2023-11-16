/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "stdoutqueuejobtest.h"
#include "misc/stdoutqueuejob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(StdoutQueueJobTest)
using namespace RocketChatRestApi;
StdoutQueueJobTest::StdoutQueueJobTest(QObject *parent)
    : QObject(parent)
{
}

void StdoutQueueJobTest::shouldHaveDefaultValue()
{
    StdoutQueueJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void StdoutQueueJobTest::shouldGenerateRequest()
{
    StdoutQueueJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/stdout.queue")));
    }
}

#include "moc_stdoutqueuejobtest.cpp"
