/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencejoinjobtest.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencejoinjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(VideoConferenceJoinJobTest)
using namespace RocketChatRestApi;
VideoConferenceJoinJobTest::VideoConferenceJoinJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceJoinJobTest::shouldHaveDefaultValue()
{
    VideoConferenceJoinJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.info().isValid());
}

void VideoConferenceJoinJobTest::shouldGenerateRequest()
{
    VideoConferenceJoinJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference.join")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void VideoConferenceJoinJobTest::shouldGenerateJson()
{
    VideoConferenceJoinJob job;
    {
        VideoConferenceJoinJob::VideoConferenceJoinInfo info;
        info.callId = QStringLiteral("bla");
        info.useCamera = false;
        info.useMicro = false;
        job.setInfo(info);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"callId":"bla","state":{"cam":false,"mic":false}})").toLatin1());
    }
    {
        VideoConferenceJoinJob::VideoConferenceJoinInfo info;
        info.callId = QStringLiteral("foo");
        info.useCamera = true;
        info.useMicro = true;
        job.setInfo(info);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"callId":"foo","state":{"cam":true,"mic":true}})").toLatin1());
    }
}

void VideoConferenceJoinJobTest::shouldNotStarting()
{
    VideoConferenceJoinJob job;

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

    VideoConferenceJoinJob::VideoConferenceJoinInfo info;
    info.callId = QStringLiteral("foo");
    info.useCamera = true;
    info.useMicro = true;
    job.setInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_videoconferencejoinjobtest.cpp"
