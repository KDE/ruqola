/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencelistjobtest.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencelistjob.h"
QTEST_GUILESS_MAIN(VideoConferenceListJobTest)
using namespace RocketChatRestApi;
VideoConferenceListJobTest::VideoConferenceListJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceListJobTest::shouldHaveDefaultValue()
{
    VideoConferenceListJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(job.roomId().isEmpty());
}

void VideoConferenceListJobTest::shouldGenerateRequest()
{
    VideoConferenceListJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference.info?roomId")));
    }
    {
        const QString roomId = QStringLiteral("foo");
        job.setRoomId(roomId);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference.info?roomId=%1").arg(roomId)));
    }
}

#include "moc_videoconferencelistjobtest.cpp"
