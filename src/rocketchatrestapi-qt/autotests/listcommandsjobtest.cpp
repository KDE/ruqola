/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listcommandsjobtest.h"
#include "commands/listcommandsjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(ListCommandsJobTest)
using namespace RocketChatRestApi;
ListCommandsJobTest::ListCommandsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ListCommandsJobTest::shouldHaveDefaultValue()
{
    ListCommandsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ListCommandsJobTest::shouldGenerateRequest()
{
    ListCommandsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/commands.list")));
}

#include "moc_listcommandsjobtest.cpp"
