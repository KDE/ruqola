/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilejobtest.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "uploadfilejob.h"
#include <QTest>
QTEST_GUILESS_MAIN(UploadFileJobTest)
using namespace RocketChatRestApi;
UploadFileJobTest::UploadFileJobTest(QObject *parent)
    : QObject(parent)
{
}

void UploadFileJobTest::shouldHaveDefaultValue()
{
    UploadFileJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.uploadFileInfo().isValid());
    QVERIFY(job.uploadFileInfo().description.isEmpty());
    QVERIFY(job.uploadFileInfo().messageText.isEmpty());
    QVERIFY(job.uploadFileInfo().roomId.isEmpty());
    QVERIFY(!job.uploadFileInfo().filenameUrl.isValid());
    QVERIFY(job.uploadFileInfo().threadMessageId.isEmpty());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());

    UploadFileJob::UploadFileInfo info;
    QVERIFY(info.filenameUrl.isEmpty());
    QVERIFY(info.roomId.isEmpty());
    QVERIFY(info.description.isEmpty());
    QVERIFY(info.messageText.isEmpty());
    QVERIFY(info.threadMessageId.isEmpty());
    QVERIFY(!info.deleteTemporaryFile);
}

void UploadFileJobTest::shouldGenerateRequest()
{
    UploadFileJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.upload")));
}

void UploadFileJobTest::shouldStart()
{
    UploadFileJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    UploadFileJob::UploadFileInfo info;
    info.filenameUrl = QUrl(QStringLiteral("file:///whatever"));
    job.setUploadFileInfo(info);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    info.roomId = QStringLiteral("bla");
    job.setUploadFileInfo(info);
    QVERIFY(job.canStart());
}
