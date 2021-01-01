/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "uploadfilejobtest.h"
#include "uploadfilejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
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
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
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
