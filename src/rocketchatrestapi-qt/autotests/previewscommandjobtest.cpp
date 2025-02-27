/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewscommandjobtest.h"
#include "commands/previewscommandjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(PreviewsCommandJobTest)
using namespace RocketChatRestApi;
PreviewsCommandJobTest::PreviewsCommandJobTest(QObject *parent)
    : QObject(parent)
{
}

void PreviewsCommandJobTest::shouldHaveDefaultValue()
{
    PreviewsCommandJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.previewsCommandInfo().isValid());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void PreviewsCommandJobTest::shouldGenerateRequest()
{
    PreviewsCommandJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/commands.preview")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void PreviewsCommandJobTest::shouldGenerateJson()
{
    PreviewsCommandJob job;
    PreviewsCommandJob::PreviewsCommandInfo info;
    info.commandName = QStringLiteral("command1");
    info.roomId = QStringLiteral("room1");
    job.setPreviewsCommandInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"command":"%1","roomId":"%2"})").arg(info.commandName, info.roomId).toLatin1());
}

void PreviewsCommandJobTest::shouldNotStarting()
{
    PreviewsCommandJob job;

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
    PreviewsCommandJob::PreviewsCommandInfo info;
    info.commandName = QStringLiteral("command1");
    job.setPreviewsCommandInfo(info);
    QVERIFY(!job.canStart());
    info.roomId = QStringLiteral("room1");
    job.setPreviewsCommandInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_previewscommandjobtest.cpp"
