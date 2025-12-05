/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencecapabilitiesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencecapabilitiesjob.h"
QTEST_GUILESS_MAIN(VideoConferenceCapabilitiesJobTest)
using namespace RocketChatRestApi;
VideoConferenceCapabilitiesJobTest::VideoConferenceCapabilitiesJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceCapabilitiesJobTest::shouldHaveDefaultValue()
{
    VideoConferenceCapabilitiesJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void VideoConferenceCapabilitiesJobTest::shouldGenerateRequest()
{
    VideoConferenceCapabilitiesJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.capabilities"_s));
    }
}

#include "moc_videoconferencecapabilitiesjobtest.cpp"
