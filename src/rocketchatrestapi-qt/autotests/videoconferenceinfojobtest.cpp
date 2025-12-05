/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferenceinfojob.h"
QTEST_GUILESS_MAIN(VideoConferenceInfoJobTest)
using namespace RocketChatRestApi;
VideoConferenceInfoJobTest::VideoConferenceInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceInfoJobTest::shouldHaveDefaultValue()
{
    VideoConferenceInfoJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.callId().isEmpty());
}

void VideoConferenceInfoJobTest::shouldGenerateRequest()
{
    VideoConferenceInfoJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.info?callId"_s));
    }
    {
        const QString callId = u"foo"_s;
        job.setCallId(callId);
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.info?callId=%1"_s.arg(callId)));
    }
}

#include "moc_videoconferenceinfojobtest.cpp"
