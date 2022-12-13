/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencenotificationjobtest.h"
#include "videoconference/videoconferencenotificationjob.h"
#include <QTest>
QTEST_MAIN(VideoConferenceNotificationJobTest)

VideoConferenceNotificationJobTest::VideoConferenceNotificationJobTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceNotificationJobTest::shouldHaveDefaultValues()
{
    VideoConferenceNotificationJob job;
    QVERIFY(!job.canStart());
    // TODO
}
