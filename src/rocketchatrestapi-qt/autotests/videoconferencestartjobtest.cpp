/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencestartjobtest.h"

#include "ruqola_restapi_helper.h"

#include "video-conference/videoconferencestartjob.h"
#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(VideoConferenceStartJobTest)
using namespace RocketChatRestApi;
VideoConferenceStartJobTest::VideoConferenceStartJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceStartJobTest::shouldHaveDefaultValue()
{
    VideoConferenceStartJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.info().isValid());
}

void VideoConferenceStartJobTest::shouldGenerateRequest()
{
    VideoConferenceStartJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.start"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void VideoConferenceStartJobTest::shouldGenerateJson()
{
    VideoConferenceStartJob job;
    VideoConferenceStartJob::VideoConferenceStartInfo info;
    info.allowRinging = false;
    info.roomId = "foo"_ba;
    info.title = u"bla"_s;
    job.setInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"allowRinging":false,"roomId":"foo","title":"bla"})"_s.toLatin1());
}

void VideoConferenceStartJobTest::shouldNotStarting()
{
    VideoConferenceStartJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    VideoConferenceStartJob::VideoConferenceStartInfo info;
    info.allowRinging = false;
    info.roomId = "foo"_ba;
    info.title = u"bla"_s;
    job.setInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_videoconferencestartjobtest.cpp"
