/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "runcommandjobtest.h"
#include "commands/runcommandjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"command":"%1","roomId":"%2","tmid":"%3"})").arg(info.commandName, info.roomId, info.threadMessageId).toLatin1());
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
