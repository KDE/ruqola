/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencestartjobtest.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencestartjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(VideoConferenceStartJobTest)
using namespace RocketChatRestApi;
VideoConferenceStartJobTest::VideoConferenceStartJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceStartJobTest::shouldHaveDefaultValue()
{
    VideoConferenceStartJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    // QVERIFY(job.permissions().isEmpty());
}

void VideoConferenceStartJobTest::shouldGenerateRequest()
{
    VideoConferenceStartJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference.start")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void VideoConferenceStartJobTest::shouldGenerateJson()
{
    VideoConferenceStartJob job;
    VideoConferenceStartJob::VideoConferenceStartInfo info;
    info.allowRinging = false;
    info.roomId = QStringLiteral("foo");
    info.title = QStringLiteral("bla");
    job.setInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"allowRinging":false,"roomId":"foo","title":"bla"})").toLatin1());
}

void VideoConferenceStartJobTest::shouldNotStarting()
{
    VideoConferenceStartJob job;

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
    VideoConferenceStartJob::VideoConferenceStartInfo info;
    info.allowRinging = false;
    info.roomId = QStringLiteral("foo");
    info.title = QStringLiteral("bla");
    job.setInfo(info);
    QVERIFY(job.canStart());
}
