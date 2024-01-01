/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencecapabilitiesjobtest.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencecapabilitiesjob.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(VideoConferenceCapabilitiesJobTest)
using namespace RocketChatRestApi;
VideoConferenceCapabilitiesJobTest::VideoConferenceCapabilitiesJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceCapabilitiesJobTest::shouldHaveDefaultValue()
{
    VideoConferenceCapabilitiesJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void VideoConferenceCapabilitiesJobTest::shouldGenerateRequest()
{
    VideoConferenceCapabilitiesJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference.capabilities")));
    }
}

#include "moc_videoconferencecapabilitiesjobtest.cpp"
