/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconfupdatejitsitimeoutjobtest.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconfupdatejitsitimeoutjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(VideoConfUpdateJitsiTimeOutJobTest)
using namespace RocketChatRestApi;
VideoConfUpdateJitsiTimeOutJobTest::VideoConfUpdateJitsiTimeOutJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConfUpdateJitsiTimeOutJobTest::shouldHaveDefaultValue()
{
    VideoConfUpdateJitsiTimeOutJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void VideoConfUpdateJitsiTimeOutJobTest::shouldGenerateRequest()
{
    VideoConfUpdateJitsiTimeOutJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference/jitsi.update-timeout")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void VideoConfUpdateJitsiTimeOutJobTest::shouldGenerateJson()
{
    VideoConfUpdateJitsiTimeOutJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

void VideoConfUpdateJitsiTimeOutJobTest::shouldNotStarting()
{
    VideoConfUpdateJitsiTimeOutJob job;

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
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}
