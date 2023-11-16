/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "runcommandjobtest.h"
#include "commands/runcommandjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(RunCommandJobTest)
using namespace RocketChatRestApi;
RunCommandJobTest::RunCommandJobTest(QObject *parent)
    : QObject(parent)
{
}

void RunCommandJobTest::shouldHaveDefaultValue()
{
    RunCommandJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.runCommandInfo().isValid());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RunCommandJobTest::shouldGenerateRequest()
{
    RunCommandJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/commands.run")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RunCommandJobTest::shouldGenerateJson()
{
    RunCommandJob job;
    RunCommandJob::RunCommandInfo info;
    info.commandName = QStringLiteral("command1");
    info.roomId = QStringLiteral("room1");
    info.threadMessageId = QStringLiteral("threadId");
    job.setRunCommandInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"command":"%1","roomId":"%2","tmid":"%3"})").arg(info.commandName, info.roomId, info.threadMessageId).toLatin1());
}

void RunCommandJobTest::shouldNotStarting()
{
    RunCommandJob job;

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
    RunCommandJob::RunCommandInfo info;
    info.commandName = QStringLiteral("command1");
    job.setRunCommandInfo(info);
    QVERIFY(!job.canStart());
    info.roomId = QStringLiteral("room1");
    job.setRunCommandInfo(info);
    QVERIFY(job.canStart());
    info.threadMessageId = QStringLiteral("threadId");
    job.setRunCommandInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_runcommandjobtest.cpp"
