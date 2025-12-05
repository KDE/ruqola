/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getcommandsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "commands/getcommandsjob.h"

#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(GetCommandsJobTest)
using namespace RocketChatRestApi;
GetCommandsJobTest::GetCommandsJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetCommandsJobTest::shouldHaveDefaultValue()
{
    GetCommandsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.commandName().isEmpty());
}

void GetCommandsJobTest::shouldGenerateRequest()
{
    GetCommandsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setCommandName(u"foo"_s);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/commands.get?command=foo"_s));
}

void GetCommandsJobTest::shouldNotStarting()
{
    GetCommandsJob job;

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
    const QString commandName = u"foo1"_s;
    job.setCommandName(commandName);
    QVERIFY(job.canStart());
}

#include "moc_getcommandsjobtest.cpp"
