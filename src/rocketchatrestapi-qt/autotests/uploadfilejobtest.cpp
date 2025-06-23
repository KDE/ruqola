/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "uploadfilejob.h"
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
    QVERIFY(info.fileName.isEmpty());
}

void UploadFileJobTest::shouldGenerateRequest()
{
    UploadFileJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.upload"_s));
}

void UploadFileJobTest::shouldStart()
{
    UploadFileJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    UploadFileJob::UploadFileInfo info;
    info.filenameUrl = QUrl(u"file:///whatever"_s);
    job.setUploadFileInfo(info);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    info.roomId = "bla"_ba;
    job.setUploadFileInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_uploadfilejobtest.cpp"
