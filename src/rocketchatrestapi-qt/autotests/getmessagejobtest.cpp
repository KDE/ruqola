/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmessagejobtest.h"
#include "chat/getmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(GetMessageJobTest)
using namespace RocketChatRestApi;
GetMessageJobTest::GetMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetMessageJobTest::shouldHaveDefaultValue()
{
    GetMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetMessageJobTest::shouldGenerateRequest()
{
    GetMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setMessageId(QStringLiteral("foo"));
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getMessage?msgId=foo")));
}

#include "moc_getmessagejobtest.cpp"
