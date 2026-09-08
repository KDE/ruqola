/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencecanceljobtest.h"

#include "ruqola_restapi_helper.h"

#include "video-conference/videoconferencecanceljob.h"
#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(VideoConferenceCancelJobTest)
using namespace RocketChatRestApi;
VideoConferenceCancelJobTest::VideoConferenceCancelJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceCancelJobTest::shouldHaveDefaultValue()
{
    VideoConferenceCancelJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.callId().isEmpty());
}

void VideoConferenceCancelJobTest::shouldGenerateRequest()
{
    VideoConferenceCancelJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.cancel"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void VideoConferenceCancelJobTest::shouldGenerateJson()
{
    VideoConferenceCancelJob job;
    job.setCallId(u"foo"_s);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"callId":"foo"})"_s.toLatin1());
}

void VideoConferenceCancelJobTest::shouldNotStarting()
{
    VideoConferenceCancelJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    job.setCallId(u"bla"_s);
    QVERIFY(job.canStart());
}

#include "moc_videoconferencecanceljobtest.cpp"
