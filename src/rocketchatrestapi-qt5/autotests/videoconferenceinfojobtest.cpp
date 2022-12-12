/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfojobtest.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferenceinfojob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(VideoConferenceInfoJobTest)
using namespace RocketChatRestApi;
VideoConferenceInfoJobTest::VideoConferenceInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceInfoJobTest::shouldHaveDefaultValue()
{
    VideoConferenceInfoJob job;
    verifyDefaultValue(&job);
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
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference.info?callId")));
    }
    {
        const QString callId = QStringLiteral("foo");
        job.setCallId(callId);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference.info?callId=%1").arg(callId)));
    }
}
