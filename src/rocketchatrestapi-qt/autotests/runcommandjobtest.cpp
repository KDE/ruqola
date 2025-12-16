/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "runcommandjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "commands/runcommandjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RunCommandJobTest)
using namespace RocketChatRestApi;
RunCommandJobTest::RunCommandJobTest(QObject *parent)
    : QObject(parent)
{
}

void RunCommandJobTest::shouldHaveDefaultValue()
{
    RunCommandJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.runCommandInfo().isValid());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RunCommandJobTest::shouldGenerateRequest()
{
    RunCommandJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/commands.run"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RunCommandJobTest::shouldGenerateJson()
{
    RunCommandJob job;
    RunCommandJob::RunCommandInfo info;
    info.commandName = u"command1"_s;
    info.roomId = u"room1"_s;
    info.threadMessageId = u"threadId"_s;
    job.setRunCommandInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"command":"%1","roomId":"%2","tmid":"%3"})").arg(info.commandName, info.roomId, info.threadMessageId).toLatin1());
}

void RunCommandJobTest::shouldNotStarting()
{
    RunCommandJob job;

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
    RunCommandJob::RunCommandInfo info;
    info.commandName = u"command1"_s;
    job.setRunCommandInfo(info);
    QVERIFY(!job.canStart());
    info.roomId = u"room1"_s;
    job.setRunCommandInfo(info);
    QVERIFY(job.canStart());
    info.threadMessageId = u"threadId"_s;
    job.setRunCommandInfo(info);
    QVERIFY(job.canStart());
}

void RunCommandJobTest::shouldHaveDefaultValueRunCommandInfo()
{
    RunCommandJob::RunCommandInfo job;
    QVERIFY(job.commandName.isEmpty());
    QVERIFY(job.roomId.isEmpty());
    QVERIFY(job.threadMessageId.isEmpty());
    QVERIFY(job.triggerId.isEmpty());
    QVERIFY(job.params.isEmpty());
    QVERIFY(!job.isValid());
}

void RunCommandJobTest::testRunCommandInfo()
{
    QFETCH(QString, commandLine);
    QFETCH(QByteArray, roomId);
    QFETCH(QByteArray, threadId);
    QFETCH(RunCommandJob::RunCommandInfo, info);
    QFETCH(bool, valid);

    const RunCommandJob::RunCommandInfo parsed = RunCommandJob::parseString(commandLine, roomId, threadId, "AUTOTEST"_L1);
    QCOMPARE(parsed.isValid(), valid);
    QCOMPARE(parsed, info);
}

void RunCommandJobTest::testRunCommandInfo_data()
{
    QTest::addColumn<QString>("commandLine");
    QTest::addColumn<QByteArray>("roomId");
    QTest::addColumn<QByteArray>("threadId");
    QTest::addColumn<RunCommandJob::RunCommandInfo>("info");
    QTest::addColumn<bool>("valid");

    {
        RunCommandJob::RunCommandInfo empty;
        QTest::addRow("empty") << QString() << QByteArray() << QByteArray() << empty << false;
    }
    {
        RunCommandJob::RunCommandInfo info;
        info.commandName = "poll"_L1;
        info.roomId = "bla"_L1;
        info.threadMessageId = "bli"_L1;
        info.triggerId = "AUTOTEST"_L1;
        // info.params;
        QTest::addRow("test1") << u"/poll"_s << QByteArray("bla") << QByteArray("bli") << info << true;
    }
    {
        RunCommandJob::RunCommandInfo info;
        info.commandName = "poll"_L1;
        info.roomId = "bla"_L1;
        info.threadMessageId = "bli"_L1;
        info.triggerId = "AUTOTEST"_L1;
        info.params = "test1"_L1;
        QTest::addRow("test2") << u"/poll test1"_s << QByteArray("bla") << QByteArray("bli") << info << true;
    }
    {
        RunCommandJob::RunCommandInfo info;
        info.commandName = "poll"_L1;
        info.roomId = "bla"_L1;
        info.threadMessageId = "bli"_L1;
        info.triggerId = "AUTOTEST"_L1;
        info.params = "test1 voiture"_L1;
        QTest::addRow("test3") << u"/poll test1 voiture"_s << QByteArray("bla") << QByteArray("bli") << info << true;
    }
    {
        RunCommandJob::RunCommandInfo info;
        info.commandName = "poll"_L1;
        info.roomId = "bla"_L1;
        info.threadMessageId = "bli"_L1;
        info.triggerId = "AUTOTEST"_L1;
        info.params = "test1 voiture"_L1;
        QTest::addRow("test4") << u"/poll test1 voiture    "_s << QByteArray("bla") << QByteArray("bli") << info << true;
    }
}

#include "moc_runcommandjobtest.cpp"
