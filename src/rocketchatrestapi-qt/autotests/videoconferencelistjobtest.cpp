/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencelistjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencelistjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(VideoConferenceListJobTest)
using namespace RocketChatRestApi;
VideoConferenceListJobTest::VideoConferenceListJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceListJobTest::shouldHaveDefaultValue()
{
    VideoConferenceListJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.list?roomId"_s));
    }
    {
        const QString roomId = u"foo"_s;
        job.setRoomId(roomId);
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.list?roomId=%1"_s.arg(roomId)));
    }
}

#include "moc_videoconferencelistjobtest.cpp"
