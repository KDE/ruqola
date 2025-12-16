/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewscommandjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "commands/previewscommandjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"command":"%1","roomId":"%2"})").arg(info.commandName, info.roomId).toLatin1());
}

void PreviewsCommandJobTest::shouldNotStarting()
{
    PreviewsCommandJob job;

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
    PreviewsCommandJob::PreviewsCommandInfo info;
    info.commandName = u"command1"_s;
    job.setPreviewsCommandInfo(info);
    QVERIFY(!job.canStart());
    info.roomId = u"room1"_s;
    job.setPreviewsCommandInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_previewscommandjobtest.cpp"
