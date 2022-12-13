/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencetest.h"
#include "videoconference/videoconference.h"
#include <QTest>
QTEST_GUILESS_MAIN(VideoConferenceTest)
VideoConferenceTest::VideoConferenceTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceTest::shouldHaveDefaultValues()
{
    VideoConference w;
    QVERIFY(w.callId().isEmpty());
    QVERIFY(w.roomId().isEmpty());
    QVERIFY(w.userId().isEmpty());
    QVERIFY(!w.isValid());
}
