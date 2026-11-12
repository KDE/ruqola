/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmessagereadreceiptsjobtest.h"

#include "chat/getmessagereadreceiptsjob.h"
#include "ruqola_restapi_helper.h"

#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(GetMessageReadReceiptsJobTest)
using namespace RocketChatRestApi;
GetMessageReadReceiptsJobTest::GetMessageReadReceiptsJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetMessageReadReceiptsJobTest::shouldHaveDefaultValue()
{
    GetMessageReadReceiptsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetMessageReadReceiptsJobTest::shouldGenerateRequest()
{
    GetMessageReadReceiptsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setMessageId("foo"_ba);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.getMessageReadReceipts?messageId=foo"_s));
}

#include "moc_getmessagereadreceiptsjobtest.cpp"
