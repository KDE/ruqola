/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listcommandsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "commands/listcommandsjob.h"

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QTest>

QTEST_GUILESS_MAIN(ListCommandsJobTest)
using namespace RocketChatRestApi;
ListCommandsJobTest::ListCommandsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ListCommandsJobTest::shouldHaveDefaultValue()
{
    ListCommandsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ListCommandsJobTest::shouldGenerateRequest()
{
    ListCommandsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/commands.list"_s));
}

#include "moc_listcommandsjobtest.cpp"
