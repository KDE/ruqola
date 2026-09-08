/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewscommandjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "commands/previewscommandjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(PreviewsCommandJobTest)
using namespace RocketChatRestApi;
PreviewsCommandJobTest::PreviewsCommandJobTest(QObject *parent)
    : QObject(parent)
{
}

void PreviewsCommandJobTest::shouldHaveDefaultValue()
{
    PreviewsCommandJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.previewsCommandInfo().isValid());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void PreviewsCommandJobTest::shouldGenerateRequest()
{
    PreviewsCommandJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/commands.preview"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void PreviewsCommandJobTest::shouldGenerateJson()
{
    PreviewsCommandJob job;
    PreviewsCommandJob::PreviewsCommandInfo info;
    info.commandName = u"command1"_s;
    info.roomId = u"room1"_s;
    job.setPreviewsCommandInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"command":"%1","roomId":"%2"})"_s.arg(info.commandName, info.roomId).toLatin1());
}

void PreviewsCommandJobTest::shouldNotStarting()
{
    PreviewsCommandJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    PreviewsCommandJob::PreviewsCommandInfo info;
    info.commandName = u"command1"_s;
    job.setPreviewsCommandInfo(info);
    QVERIFY(!job.canStart());
    info.roomId = u"room1"_s;
    job.setPreviewsCommandInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_previewscommandjobtest.cpp"
