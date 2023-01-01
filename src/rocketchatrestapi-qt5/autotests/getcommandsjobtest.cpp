/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getcommandsjobtest.h"
#include "commands/getcommandsjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(GetCommandsJobTest)
using namespace RocketChatRestApi;
GetCommandsJobTest::GetCommandsJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetCommandsJobTest::shouldHaveDefaultValue()
{
    GetCommandsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.commandName().isEmpty());
}

void GetCommandsJobTest::shouldGenerateRequest()
{
    GetCommandsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setCommandName(QStringLiteral("foo"));
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/commands.get?command=foo")));
}

void GetCommandsJobTest::shouldNotStarting()
{
    GetCommandsJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString commandName = QStringLiteral("foo1");
    job.setCommandName(commandName);
    QVERIFY(job.canStart());
}
