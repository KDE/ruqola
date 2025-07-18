/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceprovidersjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferenceprovidersjob.h"
QTEST_GUILESS_MAIN(VideoConferenceProvidersJobTest)
using namespace RocketChatRestApi;
VideoConferenceProvidersJobTest::VideoConferenceProvidersJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceProvidersJobTest::shouldHaveDefaultValue()
{
    VideoConferenceProvidersJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void VideoConferenceProvidersJobTest::shouldGenerateRequest()
{
    VideoConferenceProvidersJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.providers"_s));
    }
}

#include "moc_videoconferenceprovidersjobtest.cpp"
